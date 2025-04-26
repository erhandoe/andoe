#pragma once

namespace Andoe {

/**
 * @brief Enum to define different asychnronous models for the server.
 *
 * The server can operate in either **select** mode or **IOCP** mode.
 * Select-based mode is simpler but less scalable for high-concurrency applications.
 * IOCP mode is more efficient but requires additional setup and is more complex.
 */
enum class AsyncModel {
  ASYNC_SELECT, ///< Use select() for asynchronous IO  
  ASYNC_IOCP    ///< Use IOCP for asynchronous IO
};
}
