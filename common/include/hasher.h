#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <filesystem>

namespace sync {

    class Hasher {
        public:
            using Hash = std::array<uint8_t, 32>; // Assuming SHA-256 hash size 
            /*
             * Computes the hash of a string.
             * @param input The string to hash.
             * @return A uint8 array representing the hash of the string.
             */
            static Hash hashBuffer(const std::vector<uint8_t>& data);
    };
}