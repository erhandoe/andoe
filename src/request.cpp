#include "request.hpp"

namespace Andoe {

Request::Request() { } 

Request::Request(HttpMethod method, const std::string& path) : method(method), path(path) { }

}
