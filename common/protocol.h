#pragma once

#include <cstdint>

namespace sync {
    enum class command : uint16_t
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