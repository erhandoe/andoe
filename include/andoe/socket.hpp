#pragma once

#include "request.hpp"
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace Andoe {

/**
 * @brief Basic wrapper around WinSock SOCKET
 *
 * Automatically closes the socket when destroyed
 * Not copyable, but moveable
 */
class Socket {
  private:
    SOCKET socketHandle = INVALID_SOCKET;

  public:
    /**
     * @brief Default constructor. Creates an invalid socket.
     */
    Socket() = default;

    /**
     * @brief Constructs a socket with an existing SOCKET handle
     * @param handle A valid SOCKET handle.
     */
    explicit Socket(SOCKET handle);

    /**
     * @brief Desctructor. Closes the socket if valid.
     */
    ~Socket();
    
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;
    bool operator==(const Socket& other) const;

    /**
     * @brief Creates a socket with the given parameters.
     * @param family Adress family (e.g. AF_INET, AF_INET6)
     * @param type Socket type (e.g. SOCK_STREAM)
     * @param protocol Protocol (e.g. IPPROTO_TCP)
     * @return True if creation succeeds, false otherwise.
     */
    bool create(int family, int type, int protocol);

    /**
     * @brief Sets socket options.
     * @param level The protocol level at which the option resides.
     * @param optname The name of the option.
     * @param optval A pointer to the option value.
     * @param optlen The size of optval.
     * @return True if successful, false otherwise.
     */
    bool set_option(int level, int optname, const char* optval, int optlen);

    /**
     * @brief Closes the socket if open.
     */
    void close();

    /**
     * @brief Checks if the socket is valid.
     * @return True if valid, false otherwise.
     */
    SOCKET get_handle() const;
    
    /**
     * @brief Checks if the socket is valid.
     * @return True if valid, false otherwise.
     */
    bool is_valid() const;

    /**
     * @brief Binds the socket to a given address and port.
     * @param address The address to bind to.
     * @param addressSize The size of the address structure.
     * @return True if binding was successful, false otherwise.
     */
    bool bind_socket(const sockaddr* address, int addressSize);

    /**
     * @brief Sets the socket to listen for incoming connections.
     * @param backlog The number of pending connections.
     * @return True if successful, false otherwise.
     */
    bool listen_socket(int backlog);

    /**
     * @brief Accepts a new client connection.
     * @param clientAddr The address of the client.
     * @param clientAddrSize The size of the address structure.
     * @return A valid socket handle for the client connection.
     */
    SOCKET accept_connection(sockaddr* clientAddr, int* clientAddrSize);

    /**
     * @brief Enables or disables dual-stack mode (IPv4 + IPv6) for an IPv6 socket.
     * @param enable True to enable dual-stack, false to disable.
     * @return True if successful, false otherwise.
     */
    bool set_dual_stack(bool enable);
    
    /**
     * @brief Sends all data reliably over the socket.
     * 
     * Keeps sending until the entire buffer is transmitted or an error occurs.
     * @param data Pointer to the data buffer.
     * @param length Length of the data buffer.
     * @return True if all data was sent successfully, false otherwise.
     */
    bool send_all(const char* data, size_t length) const;

    /**
     * @brief Receives data from the socket.
     * @param buffer The buffer where the received data will be stored.
     * @param length The size of the buffer.
     * @return The number of bytes received, or -1 on error.
     */
    int recv(char* buffer, size_t length) const;
};
}
