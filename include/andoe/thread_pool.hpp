#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

namespace Andoe {

enum class ThreadPoolMode {
  Fixed,
  Dynamic
};

class ThreadPool {
  public:
    /**
     * @brief Constructor for ThreadPool.
     * @param numThreads Number of threads to create pool on
     */
    ThreadPool();
    ~ThreadPool();

    /**
     * @brief Enqueue task for the thread pool to run.
     * @param task Function to be run.
     */
    void enqueue_task(std::function<void()> task);
    

    /**
     * @brief Set the mode of the thread pool.
     * @param newMode The mode to set (Fixed or Dynamic).
     */
    void set_mode(ThreadPoolMode newMode);

    /**
     * @brief Resize the number of worker threads.
     * @param newSize New number of worker threads.
     */
    void resize(size_t newSize);

  private:
    void worker_thread();
    void resize_worker();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::mutex workerMutex;
    std::condition_variable condition;
    bool stop;
    bool resizeStop; 

    ThreadPoolMode mode;
    size_t maxThreads;
    size_t minThreads;
    size_t highThreshold;
    size_t lowThreshold;
    size_t resizeTaskQueueSize;
    size_t resizeTaskCount;

    std::thread resizeThread;
    std::mutex modeMutex;

    void enqueue_exit_task();
    void worker_exit();
};
}
