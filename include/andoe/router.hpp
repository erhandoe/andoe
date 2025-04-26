#pragma once

#include <functional>
#include <string>
#include <vector>
#include "request.hpp"
#include "response.hpp"
#include "http_method.hpp"

namespace Andoe {

/**
 * @brief Structure representing a single route in the router.
 */
using RouteHandler = std::function<void(Request&, Response&)>;

struct Route {
  HttpMethod method;
  std::string pathPattern;
  RouteHandler handler;
};

/**
 * @brief Router class that manager all HTTP routes and their handlers
 */

class Router {
  public:
    /**
     * @brief Registers a route with a specific HTTP method and handler.
     * @param method The HTTP method (GET, POST, etc.)
     * @param path The path pattern to match.
     * @param handler The function to call when the route matches.
     */
    void add_route(HttpMethod method, const std::string& path, RouteHandler handler);

    /**
     * @brief Attempts to find and execute a matching route for the given request.
     * @param request The HTTP request.
     * @param response The HTTP response to be sent back.
     * @return True if a route matched and was handled, false otherwise.
     */
    bool handle(Request& request, Response& response);
  
  private:
    std::vector<Route> routes;

    bool match_path(const std::string& pattern, const std::string& actualPath, Request& request);
};

}
