#pragma once

#include "async_model.hpp"
#include "framework.hpp"
#include "socket.hpp"
#include "router.hpp"
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

  /**
   * @brief Sets the asynchronous model used by the server. DON'T USE. OTHER MODEL NOT DONE.
   * 
   * You can choose between select-based async model or IOCP-based async model.
   * 
   * @param model The async model to use (`AsyncModel::ASYNC_SELECT` or `AsyncModel::ASYNC_IOCP`).
   */
  void set_async_model(AsyncModel model) { asyncModel = model; }

  /**
   * @brief Get access to the internal Router.
   */
  Router& get_router() { return router; }

  /**
   * @brief Add route to the server's Router
   */
  void add_route(HttpMethod method, const std::string& path, RouteHandler handler); 
private:
  AsyncModel asyncModel = AsyncModel::ASYNC_SELECT; ///< The selected async model for handling IO operations. ASYNC_SELECT is default.
  Socket serverSocket; ///<Server socket used for accepting incoming connections.
  Router router;
  std::vector<Socket> clients;

  /**
   * @brief Uses the select() function to handle asynchronous socket operations.
   * 
   * This method checks the server socket for readability using select(). When a client connects,
   * it accepts the connection and sends a simple HTTP response.
   */
  void run_select();
  
  // TODO
  void run_iocp() { } ///< Don't use, don't work;
};

}
