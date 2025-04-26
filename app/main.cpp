#include <andoe/server.hpp>

int main() {
  Andoe::Server server;

  if (!server.setup_server(80)) {
    return 1;
  }

  server.run();

  return 0;
}
