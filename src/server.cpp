#include "andoe/server.hpp"
#include <iostream>

Server::Server(int port) : port(port) {}

void Server::start() {
  std::cout << "Server is starting on port " << port << std::endl;
}
