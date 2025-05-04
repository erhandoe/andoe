#include "response.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

namespace Andoe {

Response::Response(Socket& clientSocket) : socket(clientSocket) { }

void Response::send() {
  std::string response =
    "HTTP/1.1 " + std::to_string(statusCode) + " OK\r\n"
    "Content-Type: " + contentType + "\r\n"
    "Content-Length: " + std::to_string(body.size()) + "\r\n"
    "Connection: close\r\n"
    "\r\n" +
    body;

  socket.send_all(response.c_str(), response.size());
  sent = true;
  shutdown(socket.get_handle(), SD_SEND);
}

void Response::text(int code, const std::string& bodyContent) {
  send_content(code, "text/plain", bodyContent);
}

void Response::html(int code, const std::string& bodyContent) {
  send_content(code, "text/html", bodyContent);
}

void Response::json(int code, const std::string& bodyContent) {
  send_content(code, "application/json", bodyContent);
}

void Response::static_file(int code, const std::string& path) {
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    text(404, "File not found");
    return;
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();

  std::string extension = std::filesystem::path(path).extension().string();
  std::string mime = "application/octet-stream";

  if (extension == ".html") { mime = "text/html"; }
  else if (extension == ".css") { mime = "text/css"; }
  else if (extension == ".js") { mime = "application/javascript"; }
  else if (extension == ".json") { mime = "application/json"; }
  else if (extension == ".png") { mime = "image/png"; }
  else if (extension == ".jpg" || extension == ".jpeg") { mime = "image/jpeg"; }
  else if (extension == ".gif") { mime = "image/gif"; }
  else if (extension == ".svg") { mime = "image/svg+xml"; }
  else if (extension == ".txt") { mime = "text/plain"; }

  send_content(code, mime, content);
}

void Response::send_content(int code, const std::string& type, const std::string& bodyContent) {
  statusCode = code;
  contentType = type;
  body = bodyContent;
  send();
}

bool Response::was_sent() const {
  return sent;
}

}
