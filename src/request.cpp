#include "request.hpp"
#include <sstream>

namespace Andoe {

Request::Request() { } 

Request::Request(HttpMethod method, const std::string& path) : method(method), path(path) { }

void Request::set_raw_data(const char* data, size_t length) {
    std::string raw_data(data, length);
    std::istringstream request_stream(raw_data);
    
    std::string request_line;
    std::getline(request_stream, request_line);
    
    std::istringstream line_stream(request_line);
    std::string method_str;
    std::string uri;
    std::string version;
    
    line_stream >> method_str >> uri >> version;

    // Parse method
    if (method_str == "GET") {
        method = HttpMethod::GET;
    } else if (method_str == "POST") {
        method = HttpMethod::POST;
    } else if (method_str == "PUT") {
        method = HttpMethod::PUT;
    } else if (method_str == "DELETE") {
        method = HttpMethod::DELETE_;
    } else {
        method = HttpMethod::UNKNOWN;
    }

    // Parse path and query parameters
    size_t query_pos = uri.find('?');
    if (query_pos != std::string::npos) {
        path = uri.substr(0, query_pos);
        std::string query_str = uri.substr(query_pos + 1);

        std::istringstream query_stream(query_str);
        std::string key_value_pair;
        while (std::getline(query_stream, key_value_pair, '&')) {
            auto equal_pos = key_value_pair.find('=');
            if (equal_pos != std::string::npos) {
                std::string key = key_value_pair.substr(0, equal_pos);
                std::string value = key_value_pair.substr(equal_pos + 1);
                params[key] = value;
            }
        }
    } else {
        path = uri;
    }

    // Parse headers
    std::string line;
    while (std::getline(request_stream, line) && line != "\r") {
        auto delimiter_pos = line.find(':');
        if (delimiter_pos != std::string::npos) {
            std::string header_name = line.substr(0, delimiter_pos);
            std::string header_value = line.substr(delimiter_pos + 1);

            // Trim whitespace
            if (!header_value.empty() && header_value[0] == ' ')
                header_value.erase(0, 1);
            if (!header_value.empty() && header_value.back() == '\r')
                header_value.pop_back();

            headers[header_name] = header_value;
        }
    }

    // Parse body
    auto body_start = raw_data.find("\r\n\r\n");
    if (body_start != std::string::npos) {
        body = raw_data.substr(body_start + 4);
    }
}

}
