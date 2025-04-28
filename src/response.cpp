#include "response.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

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
