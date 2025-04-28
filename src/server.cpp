#include "server.hpp"
#include "async_model.hpp"
#include "http_method.hpp"
#include <WinSock2.h>
#include <algorithm>
#include <mutex>
#include <winerror.h>
#include <ws2ipdef.h>
#include <iostream>
#include <Windows.h>
#include <sstream>

namespace Andoe {

Server::Server() = default;

Server::~Server() {
  serverSocket.close();
}

bool Server::setup_server(int port) {
  if (!initialize()) {
    return false;
  }

  if(!serverSocket.create(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) {
    std::cerr << "[Server] Failed to create socket." << std::endl;
    return false;
  }

  if (!serverSocket.set_dual_stack(true)) {
    std::cerr << "[Server] Failed to enable dual-stack mode." << std::endl;
    return false;
  }

  sockaddr_in6 addr{};
  addr.sin6_family = AF_INET6;
  addr.sin6_addr = in6addr_any;
  addr.sin6_port = htons(static_cast<u_short>(port));
  
  if (!serverSocket.bind_socket(reinterpret_cast<sockaddr*>(&addr), sizeof(addr))) {
    std::cerr << "[Server] Failed to bind socket: " << WSAGetLastError() << std::endl;
    return false;
  }

  if (!serverSocket.listen_socket(SOMAXCONN)) {
    std::cerr << "[Server] Failed to listen: " << WSAGetLastError() << std::endl;
    return false;
  }

  std::cout << "[ANDOE] Server is listening on port " << port << "..." << std::endl;
  return true;
}

void Server::run() {
  if (!serverSocket.is_valid()) {
    std::cerr << "[Server] Server socket is not valid!." << std::endl;
    return;
  }

  switch(asyncModel) {
    case AsyncModel::ASYNC_SELECT:
      run_select();
      break;
    case AsyncModel::ASYNC_IOCP:
      run_iocp();
      break;
    default:
      std::cerr << "[Server] Invalid async model selected." << std::endl;
      break;
  }

}

void Server::set_threads_number(size_t numThreads) {
  auto maxThreads = static_cast<size_t>(std::thread::hardware_concurrency());
  if (maxThreads == 0) { maxThreads = 1; }
  #undef min
  threadsNumber = std::min(numThreads, maxThreads);
  threadPool.set_mode(ThreadPoolMode::Fixed);
  threadPool.resize(threadsNumber);
}


void Server::run_select() {
  u_long mode = 1;
  ioctlsocket(serverSocket.get_handle(), FIONBIO, &mode);
  int tickCounter = 0;

  while (true) {
    fd_set readSet;
    FD_ZERO(&readSet);

    SOCKET serverHandle = serverSocket.get_handle();
    FD_SET(serverHandle, &readSet);
    SOCKET maxFd = serverHandle;

    for (auto &clientPtr : clients) {
      SOCKET h = clientPtr->get_handle();
      FD_SET(h, &readSet);
      if (h > maxFd) maxFd = h;
    }

    timeval timeout {1, 0}; // 1 second
    int count = select(int(maxFd + 1), &readSet, nullptr, nullptr, &timeout);
    if (count == SOCKET_ERROR) {
      std::cerr << "select() error: " << WSAGetLastError() << std::endl;
      continue;
    }

    if (FD_ISSET(serverHandle, &readSet)) {
      
       if (!serverSocket.is_valid()) {
          std::cerr << "Server socket is not valid before accepting connection!" << std::endl;
          continue;
       }
      sockaddr_in6 clientAddr;
      int addrSize = sizeof(clientAddr);
      SOCKET clientHandle = serverSocket.accept_connection(
          (sockaddr*)&clientAddr, &addrSize);
      if (clientHandle != INVALID_SOCKET) {
        clients.push_back(std::make_shared<Socket>(clientHandle));
      }
      else {
        int err = WSAGetLastError ();
        if (err == WSAEWOULDBLOCK) {
          continue;
        } else {
          std::cerr << "Failed to accept connection: " << WSAGetLastError() << std::endl;
          continue;
        }
      }
      
    }

    std::vector<std::shared_ptr<Socket>> stillConnected;
    {
      std::lock_guard<std::mutex> lock(clientMutex);
      for (auto &clientPtr : clients) {
        SOCKET handle = clientPtr->get_handle();
        if (FD_ISSET(handle, &readSet)) {
          auto clientCopy = clientPtr;
          threadPool.enqueue_task([this, clientCopy] {
            if (!handle_client(clientCopy)) {
              clientCopy->close();
            }
          });
        } else {
          if (clientPtr->is_valid()) {
            stillConnected.push_back(clientPtr);
          }
          else {
            clientPtr->close();
          }
        }
      }
    }
    clients.swap(stillConnected);
  }
}

bool Server::handle_client(std::shared_ptr<Socket> client) {
  char buffer[8192] = {};   
  int bytes = client->recv(buffer, sizeof(buffer)); 
  if (bytes <= 0) {
    client->close();
    client.reset();
    return false;
  }

  Request request;
  Response response(*client);

  request.set_raw_data(buffer, bytes);

  if (!router.handle(request, response)) {
    response.text(404, "Not Found");
  }
  return true;
}


void Server::add_route(HttpMethod method, const std::string& path, std::function<void(Request&, Response&)> handler) {
    router.add_route(method, path, handler); 
}
}
