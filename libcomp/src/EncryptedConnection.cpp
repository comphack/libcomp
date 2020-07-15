/**
 * @file libcomp/src/EncryptedConnection.cpp
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Lobby connection class.
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

#include "EncryptedConnection.h"

// libcomp Includes
#include "Constants.h"
#include "Crypto.h"
#include "Exception.h"
#include "Log.h"
#include "MessageConnectionClosed.h"
#include "MessageEncrypted.h"
#include "MessagePacket.h"
#include "TcpServer.h"

// object Includes
#include <ServerConfig.h>

// Standard C++11 Includes
#include <ctime>

using namespace libcomp;

EncryptedConnection::EncryptedConnection(asio::io_service& io_service)
    : libcomp::TcpConnection(io_service),
      mPacketParser(nullptr),
      mCaptureFile(nullptr) {}

EncryptedConnection::EncryptedConnection(
    asio::ip::tcp::socket& socket,
    const std::shared_ptr<Crypto::DiffieHellman>& diffieHellman)
    : libcomp::TcpConnection(socket, diffieHellman),
      mPacketParser(nullptr),
      mCaptureFile(nullptr) {}

EncryptedConnection::~EncryptedConnection() {
  if (nullptr != mCaptureFile) {
    delete mCaptureFile;
    mCaptureFile = nullptr;
  }
}

bool EncryptedConnection::Close() {
  auto messageQueue = mMessageQueue.lock();

  if (TcpConnection::Close() && messageQueue) {
    auto self = shared_from_this();

    if (nullptr != self) {
      messageQueue->Enqueue(new Message::ConnectionClosed(self));
    }

    return true;
  }

  return false;
}

void EncryptedConnection::SocketError(const libcomp::String& errorMessage) {
  if (STATUS_NOT_CONNECTED != GetStatus()) {
    LogConnectionDebug([&]() {
      return String("%1 disconnect: %2\n")
          .Arg(ROLE_CLIENT == GetRole() ? "Server" : "Client")
          .Arg(GetRemoteAddress());
    });
  }

  TcpConnection::SocketError(errorMessage);

  mPacketParser = nullptr;
}

void EncryptedConnection::ConnectionSuccess() {
  LogConnectionDebug([&]() {
    return String("%1 connection: %2\n")
        .Arg(ROLE_CLIENT == GetRole() ? "Server" : "Client")
        .Arg(GetRemoteAddress());
  });

  if (ROLE_CLIENT == GetRole()) {
    libcomp::Packet packet;

    mPacketParser = &EncryptedConnection::ParseClientEncryptionStart;

    // Now read the first reply.
    if (!RequestPacket(strlen(DH_BASE_STRING) + 2 * DH_KEY_HEX_SIZE +
                       4 * sizeof(uint32_t))) {
      SocketError("Failed to request more data.");
    }

    packet.WriteU32Big(1);
    packet.WriteU32Big(8);

    // Send a packet after connecting.
    SendPacket(packet);
  } else {
    mPacketParser = &EncryptedConnection::ParseServerEncryptionStart;

    // Read the first packet.
    if (!RequestPacket(2 * sizeof(uint32_t))) {
      SocketError("Failed to request more data.");
    }
  }
}

void EncryptedConnection::ConnectionEncrypted() {
  LogConnectionDebugMsg("Connection encrypted!\n");

  // Check if a capture file should be created.
  if (nullptr != mServerConfig.get()) {
#ifdef EXOTIC_PLATFORM
    auto capturePath = libcomp::String();
#else   // !EXOTIC_PLATFORM
    auto capturePath = mServerConfig->GetCapturePath();
#endif  // !EXOTIC_PLATFORM

    if (!capturePath.IsEmpty()) {
      std::time_t now = std::time(nullptr);
      std::tm* pTM = std::localtime(&now);

      char szTimeStamp[32];
      std::memset(szTimeStamp, 0, sizeof(szTimeStamp));

      std::strftime(szTimeStamp, sizeof(szTimeStamp), "%Y%m%d%H%M%S", pTM);

      auto captureFilePath = libcomp::String("%1/%2-%3-%4.hack")
                                 .Arg(capturePath)
                                 .Arg(szTimeStamp)
                                 .Arg(GetRemoteAddress())
                                 .Arg(rand());

      mCaptureFile = new std::ofstream();
      mCaptureFile->open(captureFilePath.C(), std::ofstream::binary);

      if (!mCaptureFile->good()) {
        delete mCaptureFile;
        mCaptureFile = nullptr;

        LogConnectionCritical([&]() {
          return String("Failed to open capture file: %1\n")
              .Arg(captureFilePath);
        });
      } else {
        auto remoteAddress = GetRemoteAddress();

        uint32_t magic = HACK_FORMAT_MAGIC;
        uint32_t version = HACK_FORMAT_VER2;
        uint64_t stamp = static_cast<uint64_t>(now);
        uint32_t addrlen = static_cast<uint32_t>(remoteAddress.Size());

        // Write the header.
        mCaptureFile->write(reinterpret_cast<char*>(&magic), sizeof(magic));
        mCaptureFile->write(reinterpret_cast<char*>(&version), sizeof(version));
        mCaptureFile->write(reinterpret_cast<char*>(&stamp), sizeof(stamp));
        mCaptureFile->write(reinterpret_cast<char*>(&addrlen), sizeof(addrlen));
        mCaptureFile->write(remoteAddress.C(), addrlen);
        mCaptureFile->flush();

        if (!mCaptureFile->good()) {
          LogConnectionCritical([&]() {
            return String("Failed to write capture file: %1\n")
                .Arg(captureFilePath);
          });

          delete mCaptureFile;
          mCaptureFile = nullptr;
        }

        LogConnectionDebug([&]() {
          return String("Started capture: %1\n").Arg(captureFilePath);
        });
      }
    }
  }

  SendMessage([](const std::shared_ptr<libcomp::TcpConnection>& self) {
    return new libcomp::Message::Encrypted(self);
  });
}

void EncryptedConnection::SendMessage(
    const std::function<libcomp::Message::Message*(
        const std::shared_ptr<libcomp::TcpConnection>&)>&
        messageAllocFunction) {
  bool errorFound = false;

  auto messageQueue = mMessageQueue.lock();

  // Check for the message queue.
  if (!errorFound && nullptr == messageQueue) {
    SocketError("No message queue for packet.");

    errorFound = true;
  }

  // Promote to a shared pointer.
  auto self = shared_from_this();

  if (!errorFound && this != self.get()) {
    SocketError("Failed to obtain a shared pointer.");

    errorFound = true;
  }

  // Notify the task about the encryption.
  if (!errorFound) {
    messageQueue->Enqueue(messageAllocFunction(self));
  }

  // Start reading until we have the packet sizes.
  if (!RequestPacket(2 * sizeof(uint32_t))) {
    SocketError("Failed to request more data.");
  }
}

void EncryptedConnection::ParseClientEncryptionStart(libcomp::Packet& packet) {
  // Check if we have all the data.
  if ((strlen(DH_BASE_STRING) + 2 * DH_KEY_HEX_SIZE + 4 * sizeof(uint32_t)) >
      packet.Size()) {
    // Keep reading the first reply.
    if (!RequestPacket(strlen(DH_BASE_STRING) + 2 * DH_KEY_HEX_SIZE +
                       4 * sizeof(uint32_t) - packet.Size())) {
      SocketError("Failed to request more data.");
    }
  } else {
    // Parsing status.
    bool status = true;

    // Sanity check the packet contents.
    if (0 != packet.ReadU32Big()) {
      SocketError("Failed to parse encryption data.");
      status = false;
    }

    // Check the size of the base.
    if (status && strlen(DH_BASE_STRING) != packet.PeekU32Big()) {
      SocketError("Failed to parse encryption base.");
      status = false;
    }

    libcomp::String base;

    if (status) {
      base = packet.ReadString32Big(libcomp::Convert::ENCODING_UTF8);
    }

    // Check the base matches what is expected.
    if (status && DH_BASE_STRING != base) {
      SocketError("Failed to parse encryption base (not " DH_BASE_STRING ").");
      status = false;
    }

    // Check the size of the prime.
    if (status && DH_KEY_HEX_SIZE != packet.PeekU32Big()) {
      SocketError("Failed to parse encryption prime.");
      status = false;
    }

    libcomp::String prime;

    if (status) {
      prime = packet.ReadString32Big(libcomp::Convert::ENCODING_UTF8);
    }

    // Check the size of the server public.
    if (status && DH_KEY_HEX_SIZE != packet.PeekU32Big()) {
      SocketError("Failed to parse encryption server public.");
      status = false;
    }

    libcomp::String serverPublic;

    if (status) {
      serverPublic = packet.ReadString32Big(libcomp::Convert::ENCODING_UTF8);
    }

    // Make sure we read the entire packet.
    if (status && 0 == packet.Left()) {
      mStatus = STATUS_WAITING_ENCRYPTION;

      // Load the prime and base.
      mDiffieHellman = libcomp::TcpServer::LoadDiffieHellman(prime);

      // Generate the client public.
      libcomp::String clientPublic =
          GenerateDiffieHellmanPublic(mDiffieHellman);

      // Generate the shared data.
      std::vector<char> sharedData =
          GenerateDiffieHellmanSharedData(mDiffieHellman, serverPublic);

      if (BF_NET_KEY_BYTE_SIZE != sharedData.size()) {
        // Get ready for the next packet.
        packet.Clear();

        SocketError(
            "Failed to generate encryption client public and shared data.");
      } else {
        libcomp::Packet reply;

        // Form the reply.
        reply.WriteString32Big(libcomp::Convert::ENCODING_UTF8, clientPublic);

        // Send the reply.
        SendPacket(reply);

        // Set the encryption key.
        SetEncryptionKey(sharedData);

        // We are now encrypted.
        mStatus = STATUS_ENCRYPTED;

        // Use this packet parser now.
        mPacketParser = &EncryptedConnection::ParsePacket;

        // Get ready for the next packet.
        packet.Clear();

        // Callback.
        ConnectionEncrypted();
      }
    } else {
      // Get ready for the next packet.
      packet.Clear();

      SocketError("Read too much data for packet.");
    }
  }
}

void EncryptedConnection::ParseServerEncryptionStart(libcomp::Packet& packet) {
  // Check if we have all the data.
  if ((2 * sizeof(uint32_t)) > packet.Size()) {
    // Keep reading the first packet.
    if (!RequestPacket(2 * sizeof(uint32_t) - packet.Size())) {
      SocketError("Failed to request more data.");
    }
  } else {
    uint32_t first = packet.ReadU32Big();
    uint32_t second = packet.ReadU32Big();

    if (0 == packet.Left() && 1 == first && 8 == second) {
      mStatus = STATUS_WAITING_ENCRYPTION;

      libcomp::Packet reply;

      reply.WriteBlank(4);
      reply.WriteString32Big(libcomp::Convert::ENCODING_UTF8, DH_BASE_STRING);
      reply.WriteString32Big(libcomp::Convert::ENCODING_UTF8,
                             GetDiffieHellmanPrime(mDiffieHellman));
      reply.WriteString32Big(libcomp::Convert::ENCODING_UTF8,
                             GenerateDiffieHellmanPublic(mDiffieHellman)
                                 .RightJustified(DH_KEY_HEX_SIZE, '0'));

      SendPacket(reply);

      mPacketParser = &EncryptedConnection::ParseServerEncryptionFinish;

      // Get ready for the next packet.
      packet.Clear();

      // Wait for the client public size (then the public).
      if (!RequestPacket(sizeof(uint32_t))) {
        SocketError("Failed to request more data.");
      }
    } else {
      // Rewind before running an extension.
      packet.Rewind();

      // Try an extension.
      if (!ParseExtensionConnection(packet)) {
        // Get ready for the next packet.
        packet.Clear();

        SocketError("Read too much data for packet.");
      }
    }
  }
}

bool EncryptedConnection::ParseExtensionConnection(libcomp::Packet& packet) {
  (void)packet;

  return false;
}

void EncryptedConnection::ParseServerEncryptionFinish(libcomp::Packet& packet) {
  // Check if we have all the data.
  if (sizeof(uint32_t) > packet.Size() ||
      (sizeof(uint32_t) + packet.PeekU32Big()) > packet.Size()) {
    // Keep reading the packet.
    if (!RequestPacket(packet.PeekU32Big() + sizeof(uint32_t) -
                       packet.Size())) {
      SocketError("Failed to request more data.");
    }
  } else {
    // Parsing status.
    bool status = true;

    // Check the size of the client public.
    if (status && DH_KEY_HEX_SIZE < packet.PeekU32Big()) {
      SocketError("Failed to parse encryption client public.");
      status = false;
    }

    libcomp::String clientPublic;

    if (status) {
      clientPublic = packet.ReadString32Big(libcomp::Convert::ENCODING_UTF8);
    }

    // Make sure we read the entire packet.
    if (status && 0 == packet.Left()) {
      std::vector<char> sharedData =
          GenerateDiffieHellmanSharedData(mDiffieHellman, clientPublic);

      if (BF_NET_KEY_BYTE_SIZE != sharedData.size()) {
        // Get ready for the next packet.
        packet.Clear();

        SocketError("Failed to generate shared data.");
      } else {
        // Set the encryption key.
        SetEncryptionKey(sharedData);

        // We are now encrypted.
        mStatus = STATUS_ENCRYPTED;

        // Use this packet parser now.
        mPacketParser = &EncryptedConnection::ParsePacket;

        // Get ready for the next packet.
        packet.Clear();

        // Callback.
        ConnectionEncrypted();
      }
    } else if (status) {
      // Get ready for the next packet.
      packet.Clear();

      SocketError("Read too much data for packet.");
    }
  }
}

void EncryptedConnection::ParsePacket(libcomp::Packet& packet) {
  (void)packet;

  if (STATUS_ENCRYPTED == GetStatus()) {
    // Check if we have all the data.
    if ((2 * sizeof(uint32_t)) > packet.Size()) {
      // Keep reading until we have the packet sizes.
      if (!RequestPacket(2 * sizeof(uint32_t) - packet.Size())) {
        SocketError("Failed to request more data.");
      }
    } else {
      // Read the sizes.
      uint32_t paddedSize = packet.ReadU32Big();
      uint32_t realSize = packet.ReadU32Big();

      // Check for enough packet data (the sizes are not included).
      if ((paddedSize + 2 * sizeof(uint32_t)) > packet.Size()) {
        // Keep reading until we have the packet.
        if (!RequestPacket(paddedSize + 2 * sizeof(uint32_t) - packet.Size())) {
          SocketError("Failed to request more data.");
        }
      } else {
        // We have a full packet, handle it now.
        ParsePacket(packet, paddedSize, realSize);

        // Get ready for the next packet.
        packet.Clear();

        // Ask for another packet now.
        if (!RequestPacket(2 * sizeof(uint32_t))) {
          SocketError("Failed to request more data.");
        }
      }
    }
  } else {
    // Get ready for the next packet.
    packet.Clear();

    SocketError("Connection should be encrypted but isn't.");
  }
}

void EncryptedConnection::ParsePacket(libcomp::Packet& packet,
                                      uint32_t paddedSize, uint32_t realSize) {
  // Decrypt the packet
  mEncryptionKey.DecryptPacket(packet);

  // Save the packet to the capture.
  if (nullptr != mCaptureFile) {
    std::time_t now = std::time(nullptr);

    uint8_t source = HACK_SOURCE_CLIENT;
    uint64_t stamp = static_cast<uint64_t>(now);
    uint64_t microtime = static_cast<uint64_t>(
        std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now())
            .time_since_epoch()
            .count());
    uint32_t size = packet.Size();

    mCaptureFile->write(reinterpret_cast<char*>(&source), sizeof(source));
    mCaptureFile->write(reinterpret_cast<char*>(&stamp), sizeof(stamp));
    mCaptureFile->write(reinterpret_cast<char*>(&microtime), sizeof(microtime));
    mCaptureFile->write(reinterpret_cast<char*>(&size), sizeof(size));
    mCaptureFile->write(packet.ConstData(), size);
    mCaptureFile->flush();

    if (!mCaptureFile->good()) {
      LogConnectionCriticalMsg("Failed to write capture file.\n");

      delete mCaptureFile;
      mCaptureFile = nullptr;
    }
  }

  // This is where to find the data.
  uint32_t dataStart = 2 * sizeof(uint32_t);

  // Decompress the packet.
  if (!DecompressPacket(packet, paddedSize, realSize, dataStart)) {
    return;
  }

  // Move the packet into a read only copy.
  ReadOnlyPacket copy(std::move(packet));

  // Make sure we are at the right spot (right after the sizes).
  copy.Seek(dataStart);

  // Calculate how much data is padding.
  uint32_t padding = paddedSize - realSize;

  // This will stop the command parsing.
  bool errorFound = false;

  // Keep reading each command (sometimes called a packet) inside the
  // decrypted packet from the network socket.
  while (!errorFound && copy.Left() > padding) {
    // Make sure there is enough data
    if (copy.Left() < 3 * sizeof(uint16_t)) {
      SocketError("Corrupt packet (not enough data for command header).");

      errorFound = true;
    } else {
      // Skip over the big endian size (we think).
      copy.Skip(2);

      // Remember where this command started so we may advance over it
      // after it has been parsed.
      uint32_t commandStart = copy.Tell();
      uint16_t commandSize = copy.ReadU16Little();
      uint16_t commandCode = copy.ReadU16Little();

      // With no data, the command size is 4 bytes (code + a size).
      if (commandSize < 2 * sizeof(uint16_t)) {
        SocketError("Corrupt packet (not enough data for command).");

        errorFound = true;
      }

      // Check there is enough packet left for the command data.
      if (!errorFound &&
          copy.Left() < (uint32_t)(commandSize - 2 * sizeof(uint16_t))) {
        SocketError("Corrupt packet (not enough data for command data).");

        errorFound = true;
      }

      // Check for the message queue.
      auto messageQueue = mMessageQueue.lock();

      if (!errorFound && nullptr == messageQueue) {
        SocketError("No message queue for packet.");

        errorFound = true;
      }

      // Promote to a shared pointer.
      auto self = shared_from_this();

      if (!errorFound && this != self.get()) {
        SocketError("Failed to obtain a shared pointer.");

        errorFound = true;
      }

      if (!errorFound) {
        // This is a shallow copy of the command data.
        ReadOnlyPacket command(
            copy, commandStart + 2 * static_cast<uint32_t>(sizeof(uint16_t)),
            commandSize - 2 * static_cast<uint32_t>(sizeof(uint16_t)));

        // Notify the task about the new packet.
        messageQueue->Enqueue(
            new libcomp::Message::Packet(self, commandCode, command));
      }

      // Move to the next command.
      if (!errorFound) {
        copy.Seek(commandStart + commandSize);
      }
    }
  }  // while(!errorFound && packet.Left() > padding)

  if (!errorFound) {
    // Skip the padding
    copy.Skip(padding);
  }

  if (!errorFound && copy.Left() != 0) {
    SocketError("Corrupt packet has extra data.");
  }
}

bool EncryptedConnection::DecompressPacket(libcomp::Packet& packet,
                                           uint32_t& paddedSize,
                                           uint32_t& realSize,
                                           uint32_t& dataStart) {
  (void)packet;
  (void)paddedSize;
  (void)realSize;
  (void)dataStart;

  return true;
}

void EncryptedConnection::PacketReceived(libcomp::Packet& packet) {
  // Pass the packet along to the parser.
  if (nullptr != mPacketParser) {
    try {
      (*this.*mPacketParser)(packet);
    } catch (libcomp::Exception& e) {
      e.Log();

      // This connection is now bad; kill it.
      SocketError();
    }
  }
}

void EncryptedConnection::SetMessageQueue(
    const std::weak_ptr<MessageQueue<libcomp::Message::Message*>>&
        messageQueue) {
  mMessageQueue = messageQueue;
}

void EncryptedConnection::SetServerConfig(
    const std::shared_ptr<objects::ServerConfig>& config) {
  mServerConfig = config;
}

void EncryptedConnection::PreparePackets(std::list<ReadOnlyPacket>& packets) {
  if (STATUS_ENCRYPTED == mStatus) {
    Packet finalPacket;

    // Reserve space for the sizes.
    finalPacket.WriteBlank(GetHeaderSize());

    // Now add the packet data.
    for (auto& packet : packets) {
      finalPacket.WriteU16Big((uint16_t)(packet.Size() + 2));
      finalPacket.WriteU16Little((uint16_t)(packet.Size() + 2));
      finalPacket.WriteArray(packet.ConstData(), packet.Size());
    }

    // Encrypt the packet
    mEncryptionKey.EncryptPacket(finalPacket);

    mOutgoing = finalPacket;
  } else if (STATUS_NOT_CONNECTED != mStatus) {
    // There should only be one!
    if (packets.size() != 1) {
      LogConnectionCriticalMsg("Critical packet error.\n");
    }

    ReadOnlyPacket finalPacket(packets.front());

    mOutgoing = finalPacket;
  }
}

std::list<ReadOnlyPacket> EncryptedConnection::GetCombinedPackets() {
  std::list<ReadOnlyPacket> packets;

  std::lock_guard<std::mutex> guard(mOutgoingMutex);

  if (!mSendingPacket) {
    uint32_t totalSize = GetHeaderSize();

    while (!mOutgoingPackets.empty() && totalSize < MAX_PACKET_SIZE) {
      ReadOnlyPacket& nextPacket = mOutgoingPackets.front();

      uint32_t packetSize =
          nextPacket.Size() + 2 * static_cast<uint32_t>(sizeof(uint16_t));

      if ((totalSize + packetSize) < MAX_PACKET_SIZE) {
        totalSize += packetSize;
        packets.push_back(mOutgoingPackets.front());
        mOutgoingPackets.pop_front();
      } else {
        // Stop parsing new packets.
        break;
      }
    }

    mSendingPacket = !packets.empty();
  }

  return packets;
}

uint32_t EncryptedConnection::GetHeaderSize() { return 2 * sizeof(uint32_t); }
