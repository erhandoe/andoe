#include <andoe/server.hpp>
#include <iostream>

int main() {
  Andoe::Server server;
  
  // Set up a route for the root path (GET "/")
  server.add_route(Andoe::HttpMethod::GET, "/", [](Andoe::Request& req, Andoe::Response& res) {
    std::cout << "[Server] Handling GET /" << std::endl;
    res.text(200, "Welcome to Andoe!"); // Sending a simple text response
  });

  // Set up a route for "/about" (GET "/about")
  server.add_route(Andoe::HttpMethod::GET, "/about", [](Andoe::Request& req, Andoe::Response& res) {
    std::cout << "[Server] Handling GET /about" << std::endl;
    res.html(200, "<body><h1>About Andoe: A modern C++ framework for SSR websites.</h1></body>");
  });

  // Set up a route for "/json" (GET "/json")
  server.add_route(Andoe::HttpMethod::GET, "/json", [](Andoe::Request& req, Andoe::Response& res) {
    res.json(200, "{\"message\": \"This is a JSON response!\"}");
  });

  // Start the server and listen on port 5000
  if (!server.setup_server(5000)) {
    std::cerr << "[Server] Failed to set up server." << std::endl;
    return 1;
  }

  server.run(); // Start the server's event loop

  return 0;
}
