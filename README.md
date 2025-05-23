# Andoe (WIP)
**Andoe** is a C++ framework for building SSR websites in React-like development experience.

## Features
- Lightweight async HTTP server (based on WinSock2).
- Full server-side rendering written in C++.
- Simple and composable API for building dynamic web pages.
- Windows supported (Linux planned)

## Build instructions
```bash
meson setup build
meson compile -C build
```

## Example usage
```cpp
#include <andoe/server.hpp>
#include <iostream>

int main() {
  Andoe::Server server;
  
  server.add_route(Andoe::HttpMethod::GET, "/", [](Andoe::Request& req, Andoe::Response& res) {
    res.text(200, "Welcome to Andoe!"); // Sending a simple text response
  });

  if (!server.setup_server(80)) {
    std::cerr << "[Server] Failed to set up server." << std::endl;
    return 1;
  }

  server.run();
  return 0;
}
```
## Project status
- [x] Basic server startup
- [x] Async client handling
- [x] Routing
- [ ] Components system
- [ ] Middleware
- [ ] Static assets

