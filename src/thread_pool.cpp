#include "thread_pool.hpp"
#include <chrono>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace Andoe {

ThreadPool::ThreadPool()
  : stop(false),
  resizeStop(false),
  mode(ThreadPoolMode::Dynamic),
  minThreads(1),
  highThreshold(8),
  lowThreshold(2) {
    auto n = std::thread::hardware_concurrency();
    if (n == 0) {
      std::cerr << "Couldn't find number of cores, setting maxThreads to 2. It's reccomended to go to fixed threading.";
    }
    maxThreads = std::max(static_cast<size_t>(2), static_cast<size_t>(n));

    resizeThread = std::thread(&ThreadPool::resize_worker, this);
    
    for (size_t i = 0; i < (maxThreads / 2); ++i) {
      workers.emplace_back(&ThreadPool::worker_thread, this);
    }
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    stop = true;
  }
  condition.notify_all();

  for (auto& worker : workers) {
    if (worker.joinable()) { worker.join(); }
  }

  {
    std::lock_guard<std::mutex> lock(modeMutex);
    resizeStop = true;
  }

  if (resizeThread.joinable()) { resizeThread.join(); }
}

void ThreadPool::enqueue_task(std::function<void()> task) {
  {
    //std::lock_guard<std::mutex> lock(queueMutex);
    std::unique_lock<std::mutex> lock(queueMutex);
    taskQueue.push(task);
  }
  //condition.notify_all();
  condition.notify_one();
}

void ThreadPool::worker_thread() {
  while (!stop) {
    //std::cout << "TEST1 " << "workers: " << workers.size() << std::endl;
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      condition.wait(lock, [this] { return stop || !taskQueue.empty(); });

      if (stop && taskQueue.empty()) { return; }
      task = taskQueue.front();
      taskQueue.pop();
    }
    if(!task) {
      //std::cout << "[ThreadPool] Worker received exit signal.\n";
      worker_exit();
      return;
    }
    task();
  }
}

void ThreadPool::set_mode(ThreadPoolMode newMode) {
  std::lock_guard<std::mutex> lock(queueMutex);
  mode = newMode;

  if (mode == newMode) { return; }

  if (mode == ThreadPoolMode::Fixed) {
    resizeStop = true;
    if (resizeThread.joinable()) { resizeThread.join(); }
    resize(workers.size());
  }

  else if (newMode == ThreadPoolMode::Dynamic) {
    if (!resizeThread.joinable()) {
      resizeStop = false;
      resizeThread = std::thread(&ThreadPool::resize_worker, this);
    }
  }

  mode = newMode;
}

void ThreadPool::resize(size_t newSize) {
  if (newSize < minThreads || newSize > maxThreads) { return; }

  std::lock_guard<std::mutex> lock(workerMutex);
  size_t current = workers.size();

  if (newSize > current) {
    for (size_t i = 0; i < (newSize - current); ++i) {
      workers.emplace_back(&ThreadPool::worker_thread, this);
    }
  }
  else if (newSize < current) {
    size_t count = current - newSize;
    for (size_t i = 0; i < count; ++i) {
      enqueue_exit_task();
    }
  }

  std::cout << "[ThreadPool] Resizing thread pool to " << newSize << " threads." << std::endl;
}

void ThreadPool::resize_worker() {
  while (!resizeStop) {  
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    size_t taskCount = taskQueue.size();

    if (mode == ThreadPoolMode::Dynamic) {
      if (taskCount > highThreshold) {
        size_t newThreads = workers.size() + 1;
        if (newThreads <= maxThreads) {
          resize(newThreads);
        }
      }
      else if (taskCount < lowThreshold && workers.size() > minThreads) {
        size_t newThreads = workers.size() - 1;
        if (newThreads >= minThreads) {
          resize(newThreads);
        }
      }
    }
  }
}

void ThreadPool::enqueue_exit_task() {
  {
    std::unique_lock<std::mutex> lock(queueMutex);
    taskQueue.push(nullptr);
  }
  condition.notify_one();
}

void ThreadPool::worker_exit() {
  std::lock_guard<std::mutex> lock(workerMutex);
  auto it = std::find_if(workers.begin(), workers.end(), [](std::thread& t) {
    return t.get_id() == std::this_thread::get_id();
  });

  if (it != workers.end()) {
    if (it->joinable()) {
      it->detach();
    }
    workers.erase(it);
  }
}

}
