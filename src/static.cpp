#include "static.hpp"
#include <server.hpp>
#include <filesystem>
#include <iostream>
#include <string>

namespace Andoe {

void serve_static_folder(Server& server, const std::string& folder, const std::string& mountPath) {
  std::string routePrefix = mountPath;
  if (routePrefix.empty() || routePrefix[0] != '/')
    routePrefix = '/' + routePrefix;
  if (routePrefix.back() != '/') {
    routePrefix += '/';
  }

  namespace fs = std::filesystem;

  for (const auto& entry : fs::recursive_directory_iterator(folder)) {
    if (!entry.is_regular_file()) continue;

    std::string fullPath = entry.path().string();
    std::string relativePath = fs::relative(entry.path(), folder).string();

    // Delete extension
    std::string extension = fs::path(relativePath).extension().string();
    if (!extension.empty()) {
      relativePath = relativePath.substr(0, relativePath.length() - extension.length());
    }

    std::string route = routePrefix + relativePath;

    if (relativePath == "index")
      route = routePrefix + "";

    server.add_route(HttpMethod::GET, route, [fullPath](Request& req, Response& res) {
      res.static_file(200, fullPath);
    });

    std::cout << "[Static] Serving " << fullPath << " at " << route << "\n";
  }
}
}
