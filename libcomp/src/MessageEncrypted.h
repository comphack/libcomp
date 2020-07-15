/**
 * @file libcomp/src/MessageEncrypted.h
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Connection encrypted message.
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

#ifndef LIBCOMP_SRC_MESSAGEENCRYPTED_H
#define LIBCOMP_SRC_MESSAGEENCRYPTED_H

// libcomp Includes
#include "ConnectionMessage.h"

// Standard C++11 Includes
#include <memory>

namespace libcomp {

class TcpConnection;

namespace Message {

/**
 * Message signifying that a connection has been encrypted.  This
 * acts as a signal to start sending messages between servers.
 */
class Encrypted : public ConnectionMessage {
 public:
  /**
   * Create the message.
   * @param connection The encrypted connection
   */
  Encrypted(const std::shared_ptr<TcpConnection>& connection);

  /**
   * Cleanup the message.
   */
  virtual ~Encrypted();

  /**
   * Get the encrypted connection.
   * @return The encrypted connection
   */
  std::shared_ptr<TcpConnection> GetConnection() const;

  virtual ConnectionMessageType GetConnectionMessageType() const;

  virtual libcomp::String Dump() const override;

 private:
  /// The encrypted connection
  std::shared_ptr<TcpConnection> mConnection;
};

}  // namespace Message

}  // namespace libcomp

#endif  // LIBCOMP_SRC_MESSAGEENCRYPTED_H
