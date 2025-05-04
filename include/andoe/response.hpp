#pragma once

#include <string>
#include "socket.hpp"

namespace Andoe {

/**
 * @brief Represent an HTTP response sent to the client
 */
class Response {
  public:
    explicit Response(Socket& clientSocket);

    /**
     * @brief send_content shortcut for plain text.
     * @param code HTTP code for response.
     * @param body Body of the response.
     */
    void text(int code, const std::string& body);

    /**
     * @brief send_content shortcut for html.
     * @param code HTTP code for response.
     * @param body Body of the response.
     */
    void html(int code, const std::string& body);

    /**
     * @brief send_content shortcut for JSON.
     * @param code HTTP code for response.
     * @param body Body of the response.
     */
    void json(int code, const std::string& body);

    /**
     * @brief send_content shortcut for static files.
     * @param code HTTP code for response.
     * @param path Path to the file to send.
     */
    void static_file(int code, const std::string& path);

    /**
     * @brief Send content as response with provided type
     * @param code HTTP code for response.
     * @param contentType Type of content for the response (e.g. 'text/html')
     * @param body Body of the response.
     */
    void send_content(int code, const std::string& contentType, const std::string& body);

    bool was_sent() const;

  private:
    /**
     * @brief Sends response.
     * @param body The content to send
     */
    void send();

    bool sent = false;

    Socket& socket;
    int statusCode = 200;
    std::string body;
    std::string contentType = "text/plain";
};

}
