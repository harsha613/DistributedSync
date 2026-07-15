#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../common/protocol.h"

namespace sync {

    class Network {
    public:

    Network();
    ~Network();

    Network(const Network&) = delete;
    Network& operator=(const Network&) = delete;

    bool connect(const std::string& ip, uint16_t port);

    void disconnect();

    bool isconnected() const;

    bool sendPacket(Command command, const std::vector<uint8_t>& payload);

    bool receivePacket(PacketHeader& header, std::vector<uint8_t>& payload);

    private:
        bool sendAll(const uint8_t* data, size_t size);
        bool receiveAll(uint8_t* data, size_t size);

    private:
        int socketFd;
        bool connected;
    };
}