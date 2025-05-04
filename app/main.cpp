#include "andoe/htmlTagFactories.hpp"
#include <andoe/andoe.hpp>
#include <iostream>

using namespace Andoe::Html;

int main() {
  Andoe::Server server;

  server.add_route(Andoe::HttpMethod::GET, "/", [](Andoe::Request& req, Andoe::Response& res) {
    auto page = HtmlElement("html", {
        Head({
          Title("Main Site"),
          Meta().attr("description", "Test uwu")
        }),
        Body({
          Div({
            H1("Welcome to the Main Site"),
            P("This is the main site.")
          }),
          Div({
            H2("About"),
            P("This is the main site.")
          })
        })
      });

    res.html(200, page.str());
  });
  if (!server.setup_server(5000)) {
    std::cerr << "[Server] Failed to set up server." << std::endl;
    return 1;
  }

  server.run();

  return 0;
}
