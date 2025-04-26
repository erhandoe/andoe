#pragma once

#include <string>

class Server {
  public:
    Server(int port);
    void start();
  private:
    int port;
};
