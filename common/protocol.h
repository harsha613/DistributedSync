#pragma once

#include <arpa/inet.h>
#include <cstdint>

namespace sync {

    inline uint64_t htonll(uint64_t value) {
        #if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
            return (((uint64_t)htonl(value & 0xFFFFFFFF)) << 32) | htonl(value >> 32);
        #else
            return value;
        #endif
    }

    inline uint64_t ntohll(uint64_t value) {
        #if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
            return (((uint64_t)ntohl(value & 0xFFFFFFFF)) << 32) | ntohl(value >> 32);
        #else
            return value;
        #endif
    }

    enum class Command : uint16_t
    {
        LOGIN = 1,
        LOGIN_RESPONSE,

        SYNC_REQUEST,
        SYNC_RESPONSE,

        FILE_UPLOAD,
        FILE_DOWNLOAD,

        FILE_DELETE,

        FILE_LIST,

        HEARTBEAT,

        ERROR_MESSAGE
    };

    /*
    Header of packet with layout as below
    | command(2) | reserved(2) | payloadSize(4) | requestId(8) |
    */
    struct PacketHeader
    {
        uint16_t command;
        uint16_t reserved;
        uint32_t payloadSize;
        uint64_t requestId;
    };

    static_assert(sizeof(PacketHeader) == 16, "Packet header must be exactly 16 bytes.");

} // namespace sync