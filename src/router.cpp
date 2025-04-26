#include "router.hpp"
#include <sstream>

namespace Andoe {

void Router::add_route(HttpMethod method, const std::string& path, RouteHandler handler) {
    routes.push_back(Route{method, path, handler});
}

bool Router::handle(Request& request, Response& response) {
  for (auto& route : routes) {
    if (route.method == request.method && match_path(route.pathPattern, request.path, request)) {
      route.handler(request, response);
      return true;
    }
  }
  return false;
}



bool Router::match_path(const std::string& pattern, const std::string& actualPath, Request& request) {
  std::istringstream patternStream(pattern);
  std::istringstream pathStream(actualPath);
  std::string patternSegment, pathSegment;

  // Handle the root path case directly
  if (pattern == "/" && actualPath == "/") { return true; }

  while (std::getline(patternStream, patternSegment, '/') && std::getline(pathStream, pathSegment, '/')) {
    if (patternSegment.empty() && pathSegment.empty()) { continue; }

    if (!patternSegment.empty() && patternSegment[0] == ':') {
      std::string paramName = patternSegment.substr(1);
      request.params[paramName] = pathSegment;
    }
    else{
      if (patternSegment != pathSegment) { return false; }
    }
  }

  return patternStream.eof() && pathStream.eof();
}

}
