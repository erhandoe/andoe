#include "server.hpp"
#include "async_model.hpp"
#include <WinSock2.h>
#include <cstdint>
#include <errhandlingapi.h>
#include <handleapi.h>
#include <ioapiset.h>
#include <winbase.h>
#include <ws2ipdef.h>
#include <iostream>
#include <Windows.h>

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

void Server::run_select() {
  fd_set readfds;

  while(true) {
    FD_ZERO(&readfds);
    FD_SET(serverSocket.get_handle(), &readfds);

    timeval timeout = {1, 0}; // 1 second
    
    int activity = select(0, &readfds, nullptr, nullptr, &timeout);
    
    if (activity < 0) {
      std::cerr << "[Server] select() error: " << WSAGetLastError() << std::endl;
      continue;
    }

    if (FD_ISSET(serverSocket.get_handle(), &readfds)) {
      sockaddr_storage clientAddr;
      int clientAddrSize = sizeof(clientAddr);
      SOCKET clientHandle = accept(serverSocket.get_handle(), reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
      if (clientHandle == INVALID_SOCKET) {
        std::cerr << "[Server] Failed to accept client: " << WSAGetLastError() << std::endl;
        continue;
      }

      Socket client(std::move(clientHandle));

      char clientIp[INET6_ADDRSTRLEN] = {};
      uint16_t clientPort = 0;

      if (clientAddr.ss_family == AF_INET) {
        sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(&clientAddr);
        inet_ntop(AF_INET, &(addr_in->sin_addr), clientIp, INET_ADDRSTRLEN);
        clientPort = ntohs(addr_in->sin_port);
      } else if (clientAddr.ss_family == AF_INET6) {
        sockaddr_in6* addr_in6 = reinterpret_cast<sockaddr_in6*>(&clientAddr);
        inet_ntop(AF_INET6, &(addr_in6->sin6_addr), clientIp, INET6_ADDRSTRLEN);
        clientPort = ntohs(addr_in6->sin6_port);
      }

      std::cout << "[Server] New client connected: " << clientIp << ":" << clientPort << std::endl;

      const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello, World!";

      if (!client.send_all(response, strlen(response))) {
        std::cerr << "[Server] Failed to fully send data." << std::endl;
      }

      shutdown(client.get_handle(), SD_SEND); 
    }
  }
}

void Server::run_iocp() {
  HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
  if (iocpHandle == nullptr) {
    std::cerr << "[Server] Failed to create IOCP: " << GetLastError() << std::endl;
    return;
  }

  if (CreateIoCompletionPort((HANDLE)serverSocket.get_handle(), iocpHandle, 0, 0) == nullptr) {
    std::cerr << "[Server] Failed tp associate socket with IOCP: " << GetLastError() << std::endl;
    return;
  }

  std::cout << "[Server] IOCP server is ready to accept connections..." << std::endl;


  while (true) {
    DWORD bytesTransferred = 0;
    ULONG_PTR completionKey = 0;
    LPOVERLAPPED pOverlapped = nullptr;

    BOOL result = GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, &completionKey, &pOverlapped, INFINITE);
    if (!result) {
      std::cerr << "[Server] Failed to get completed IO operation: " << GetLastError() << std::endl;
      continue;
    }

  if (bytesTransferred == 0) {
    std::cerr << "[Server] No bytes transferred. Connection failed or was aborted." << std::endl;
    continue;
  }

  std::cout << "[Server] New client connected via IOCP" << std::endl;  } 
}

}
