#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace Andoe {

/**
 * @brief Handles initialization and cleanup of the networking framework.
 *
 * Manages Winsock startup and shutdown, and automatically handles
 * safe closing of a server socket.
 */
class Framework {
  public:
    /**
     * @brief Constructor. Prepares internal structures.
     */
    Framework();
     /**
     * @brief Destructor. Cleans up resources.
     */
    ~Framework();

    /**
     * @brief Initializes Winsock.
     *
     * Must be called before starting network operations.
     * @return True if initialized successfully, false otherwise.
     */
    bool initialize();

    /**
     * @brief Cleans up Winsock
     */
    void cleanup();
  private:
    WSADATA wsaData; ///< Winsock data structure
    bool initialized = false; ///<Tracks if Winsock was successfully initialized.
};

}
