# Andoe
**Andoe** is a modern C++ framework for building SSR websites in React-like development experience.

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
    server.listen(5000);
}
```
## Project status
- [x] Basic server startup
- [x] Async client handling
- [ ] Routing
- [ ] Components system
- [ ] Middleware
- [ ] Static assets

