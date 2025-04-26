# Andoe (WIP - most things dont exist yet)
**Andoe** is a C++ framework for building SSR websites in React-like development experience.

## Features
- Lightweight async HTTP server (based on WinSock2).
- Full server-side rendering written in C++.
- Simple and composable API for building dynamic web pages.
- Windows supported (Linux planned)

## Build instructions
```bash
meson setup building
meson compile -C build
```

## Example usage
```cpp
#include <andoe/server.hpp>

int main() {
  Andoe::Server server;

  if (!server.setup_server(80)) {
    return 1;
  }

  server.run();

  return 0;
}
```
## Project status
- [x] Basic server startup
- [x] Async client handling
- [ ] Routing
- [ ] Components system
- [ ] Middleware
- [ ] Static assets

