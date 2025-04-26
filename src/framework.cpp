#include "framework.hpp"
#include <WinSock2.h>
#include <iostream>

namespace Andoe {

Framework::Framework() = default;

Framework::~Framework() {
  cleanup();
}

bool Framework::initialize() {
  int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result != 0) {
    std::cerr << "[Framework] Failed to initialize Winsock: " << result << std::endl;
    return false;
  }
  initialized = true;
  return true;
}

void Framework::cleanup() {
  if (initialized) {
    WSACleanup();
    initialized = false;
  }
}

}
