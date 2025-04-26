#include "server.hpp"
#include "async_model.hpp"
#include "http_method.hpp"
#include <WinSock2.h>
#include <algorithm>
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


void Server::run_select() {
  u_long mode = 1;
  ioctlsocket(serverSocket.get_handle(), FIONBIO, &mode);
  // Timeout interval
  timeval timeout = {1, 0}; // 1 second

  while (true) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(serverSocket.get_handle(), &readfds);

    for (auto& client : clients) {
      FD_SET(client.get_handle(), &readfds);
    }

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
      // Create a new Socket object for the client and add it to the list of clients
      Socket client(std::move(clientHandle));
      clients.push_back(std::move(client));
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
    }
    
    std::vector<Socket> closedClients;

    for (auto& client : clients) {
      char buffer[4096] = {};
      int bytesRead = client.recv(buffer, sizeof(buffer));
      if (bytesRead == 0) {
        closedClients.push_back(std::move(client));
        continue;
      } 
      else if (bytesRead < 0) {
        std::cerr << "[Server] Error receiving data: " << WSAGetLastError() << std::endl;
        closedClients.push_back(std::move(client));
        continue;
      }
      buffer[bytesRead] = '\0';

      std::string methodStr, path;
      {
        std::stringstream requestLine(buffer);
        requestLine >> methodStr >> path;
      }

      HttpMethod method = HttpMethod::GET;
      if (methodStr == "GET") method = HttpMethod::GET;
      else if (methodStr == "POST") method = HttpMethod::POST;
      else if (methodStr == "PUT") method = HttpMethod::PUT;
      else if (methodStr == "DELETE") method = HttpMethod::DELETE_;
      else if (methodStr == "PATCH") method = HttpMethod::PATCH;
      Request request(method, path);
      Response response(client);

      bool handled = router.handle(request, response);

      if (!handled) {
        const char* notFound =
          "HTTP/1.1 404 Not Found\r\n"
          "Content-Type: text/plain\r\n"
          "Content-Length: 9\r\n"
          "Connection: close\r\n"
          "\r\n"
          "Not Found";
        client.send_all(notFound, strlen(notFound));
      }

      shutdown(client.get_handle(), SD_SEND);
      //closedClients.push_back(std::move(client));
    }

    for (auto& client : closedClients) {
      shutdown(client.get_handle(), SD_SEND);
      client.close();
      clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
    }
  }
}
void Server::add_route(HttpMethod method, const std::string& path, std::function<void(Request&, Response&)> handler) {
    router.add_route(method, path, handler); 
}
}
