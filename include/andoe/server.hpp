#pragma once

#include "framework.hpp"
#include "socket.hpp"
#include <WinSock2.h>

namespace Andoe {

/**
 * @brief Server class to handle the creation and operation of a network server.
 * 
 * The Server class inherits from the Framework, which manages Winsock initialization
 * and cleanup. It manages the setup and handling of a server socket for accepting
 * client connections.
 */
class Server : public Framework {
public:
    /**
     * @brief Constructor for the Server class.
     * 
     * Initializes server socket state.
     */
    Server();

    /**
     * @brief Destructor for the Server class.
     * 
     * Ensures cleanup of server socket resources.
     */
    ~Server();

    /**
     * @brief Sets up the server socket to listen on the specified port.
     * 
     * Creates the socket, binds it, and prepares it for listening for client connections.
     * @param port The port number the server should listen on.
     * @return True if the setup was successful, false otherwise.
     */
    bool setup_server(int port);

    /**
     * @brief Runs the server, accepting incoming client connections.
     * 
     * This method continuously accepts incoming client connections and handles them.
     * The actual handling of requests would be implemented here.
     */
    void run();

private:
    Socket serverSocket; ///<Server socket used for accepting incoming connections.
};

}
