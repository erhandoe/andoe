#include "socket.hpp"
#include "request.hpp"
#include <WinSock2.h>
#include <iostream>
#include <ws2ipdef.h>

namespace Andoe {

Socket::Socket(SOCKET handle) : socketHandle(handle) { }

Socket::~Socket() {
  if (is_valid()) {
    close();
  }
}

Socket::Socket(Socket&& other) noexcept : socketHandle(other.socketHandle) {
  other.socketHandle = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& other) noexcept {
  if (this != &other) {
    if (is_valid()) {
      close();
    }
    socketHandle = other.socketHandle;
    other.socketHandle = INVALID_SOCKET;
  }
  return *this;
}

bool Socket::operator==(const Socket& other) const {
  return this->socketHandle == other.socketHandle;
}

bool Socket::create(int family, int type, int protocol) {
  socketHandle = ::socket(family, type, protocol);
  if (!is_valid()) {
    std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

bool Socket::set_option(int level, int optname, const char* optval, int optlen) {
  if (::setsockopt(socketHandle, level, optname, optval, optlen) == SOCKET_ERROR) {
    std::cerr << "Failed to set socket option: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

void Socket::close() {
  if(is_valid()) {
    ::closesocket(socketHandle);
    socketHandle = INVALID_SOCKET;
  }
}

SOCKET Socket::get_handle() const {
  return socketHandle;
}

bool Socket::is_valid() const {
  return socketHandle != INVALID_SOCKET;
}

bool Socket::bind_socket(const sockaddr* adress, int adressSize) {
  if(::bind(socketHandle, adress, adressSize) == SOCKET_ERROR) {
    std::cerr << "Failed to bind socket: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

bool Socket::listen_socket(int backlog) {
  if (::listen(socketHandle, backlog) == SOCKET_ERROR) {
    std::cerr << "Failed to listen on socket: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

SOCKET Socket::accept_connection(sockaddr* clientAddr, int* clientAddrSize) {
  SOCKET clientSocket = ::accept(socketHandle, clientAddr, clientAddrSize);
  if(clientSocket == INVALID_SOCKET) {
    std::cerr << "Failed to accept connection: " << WSAGetLastError() << std::endl;
  }
  return clientSocket;
}

bool Socket::set_dual_stack(bool enable) {
  int value = enable ? 0 : 1;
  if (::setsockopt(socketHandle, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&value, sizeof(value)) == SOCKET_ERROR) {
    std::cerr << "Failed to set dual-stack mode: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

bool Socket::send_all(const char* data, size_t length) const {
  if (!is_valid()) return false;
    size_t totalSent = 0;
    while (totalSent < length) {
      int bytesSent = send(socketHandle, data + totalSent, static_cast<int>(length - totalSent), 0);
        if (bytesSent == SOCKET_ERROR) {
          std::cerr << "[Socket] Send error: " << WSAGetLastError() << std::endl;
          return false;
      }
      totalSent += bytesSent;
    }
  return true;
}

int Socket::recv(char* buffer, size_t length) const {
  if (!is_valid()) {
    std::cerr << "[Server] Invalid socket handle." << std::endl;
    return -1;
  }

  int bytesReceived = ::recv(socketHandle, buffer, static_cast<int>(length), 0);
  if (bytesReceived == SOCKET_ERROR) {
    int errorCode = WSAGetLastError();
    if (errorCode == WSAEWOULDBLOCK) { return 0; }
      std::cerr << "[Server] Recv failed: " << errorCode << std::endl;
    }

    return bytesReceived;
  }
}
