#pragma once

#include "server.hpp"
#include <string>

namespace Andoe {

void serve_static_folder(Server& server, const std::string& path, const std::string& mountPath = "/");
}
