/**
 * @file libcomp/src/Shutdown.h
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Shutdown signal handler.
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

#ifndef LIBCOMP_SRC_SHUTDOWN_H
#define LIBCOMP_SRC_SHUTDOWN_H

#ifndef EXOTIC_PLATFORM

void ShutdownSignalHandler(int sig);

namespace libcomp {

class BaseServer;

namespace Shutdown {

/**
 * Configure a server at runtime to to handle any type of termination
 * that should result in executing the @ref BaseServer::Shutdown.
 * @param pServer The server to handle messages for
 */
void Configure(libcomp::BaseServer *pServer);

/**
 * Clean up the server reference and delete any threads used to handle
 * program termination.
 */
void Complete();

}  // namespace Shutdown

}  // namespace libcomp

#endif  // !EXOTIC_PLATFORM

#endif  // LIBCOMP_SRC_SHUTDOWN_H
