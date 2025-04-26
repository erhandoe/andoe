#pragma once

#include "http_method.hpp"
#include <string>
#include <unordered_map>

namespace Andoe {

class Request {
  public:
    HttpMethod method = HttpMethod::UNKNOWN; ///< HTTP method of the request
    std::string path;
    std::unordered_map<std::string, std::string> params;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    Request();
    Request(HttpMethod method, const std::string& path);
};

}
