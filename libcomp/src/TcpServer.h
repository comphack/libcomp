/**
 * @file libcomp/src/TcpServer.h
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Base TCP/IP server class.
 *
 * This file is part of the COMP_hack Library (libcomp).
 *
 * Copyright (C) 2012-2020 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBCOMP_SRC_TCPSERVER_H
#define LIBCOMP_SRC_TCPSERVER_H

// libcomp Includes
#include "CString.h"
#include "Crypto.h"

// Ignore warnings
#include "PushIgnore.h"

// Boost ASIO Includes
#include <asio.hpp>

// Stop ignoring warnings
#include "PopIgnore.h"

// Standard C++ Includes
#include <memory>
#include <mutex>
#include <thread>

namespace libcomp {

class TcpConnection;

/**
 * Listen for new TCP/IP connections. This class will listen for new TCP/IP
 * connections on the given address and port. If the address specified is blank
 * or "any" it will listen for connections on all network devices. The
 * @ref CreateConnection function should be overridden if a subclass of
 * @ref TcpConnection is required or additional setup should be performed.
 */
class TcpServer {
 public:
  /**
   * Create a TCP server to listen on a specific address and port.
   * @param listenAddress Listen on the specified IP address. If blank or
   * "any" this will listen on all network adapters.
   * @param port Port to listen for incoming connections.
   */
  TcpServer(const String& listenAddress, uint16_t port);

  /**
   * Cleanup the server and stop listening for new connections.
   */
  virtual ~TcpServer();

  /**
   * Start a thread that listens for incoming network connections. This call
   * will block until the server is stopped. This will call @ref Run for the
   * main loop. In the @ref BaseServer implementation this will run a main
   * worker thread.
   * @sa BaseServer
   * @sa Worker
   * @param delayReady Optional parameter to delay ready notification to
   *  be called manually at a later point. Defaults to false.
   * @return Code indicating the exit status.
   */
  virtual int Start(bool delayReady = false);

  /**
   * Remove a connection from the list of client connections.
   * @param connection Connection to remove.
   */
  void RemoveConnection(std::shared_ptr<TcpConnection>& connection);

  /**
   * Generate a Diffie-Hellman key pair.
   * @return Generated key pair or nullptr on failure.
   */
  static std::shared_ptr<Crypto::DiffieHellman> GenerateDiffieHellman();

  /**
   * Create a Diffie-Helman key pair given the hex encoded prime.
   * @param prime Hex encoded string representing the prime.
   * @return Generated key pair or nullptr on failure.
   */
  static std::shared_ptr<Crypto::DiffieHellman> LoadDiffieHellman(
      const String& prime);

  /**
   * Called when the server has started.
   */
  virtual void ServerReady();

 protected:
  /**
   * Main loop for the server.
   * @return Code indicating the exit status.
   */
  virtual int Run();

  /**
   * Create a connection to a newly active socket.
   * @param socket A new socket connection.
   * @return Pointer to the newly created connection
   */
  virtual std::shared_ptr<TcpConnection> CreateConnection(
      asio::ip::tcp::socket& socket);

  /**
   * Get the Diffie-Hellman key pair used by this server.
   * @return Key pair or nullptr if one is not set.
   */
  std::shared_ptr<Crypto::DiffieHellman> GetDiffieHellman() const;

  /**
   * Set the Diffie-Hellman key pair used by this server.
   * @param diffieHellman Pointer to the key pair to use.
   */
  void SetDiffieHellman(
      const std::shared_ptr<Crypto::DiffieHellman>& diffieHellman);

  /**
   * Called to handle a new connection to the server. This will call
   * @ref CreateConnection and then add the connection to the list.
   * @param errorCode Error code of the last accept operation.
   * @param socket Socket that was bound to the new connection.
   */
  void AcceptHandler(asio::error_code errorCode, asio::ip::tcp::socket& socket);

  /// Lock for the connection list.
  std::mutex mConnectionsLock;

  /// List of connections managed by this server.
  std::list<std::shared_ptr<TcpConnection>> mConnections;

  /// ASIO service used to handle network operations.
  asio::io_service mService;

 private:
  /// Asynchronous acceptor for new connections.
  asio::ip::tcp::acceptor mAcceptor;

  /// Thread that runs the ASIO service.
  std::thread mServiceThread;

  /// Diffie-Hellman key pair used to encrypt connections.
  std::shared_ptr<Crypto::DiffieHellman> mDiffieHellman;

  /// Address the server is listening on.
  String mListenAddress;

  /// Port the address is listening on.
  uint16_t mPort;
};

}  // namespace libcomp

#endif  // LIBCOMP_SRC_TCPSERVER_H
