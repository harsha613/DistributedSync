#include "../include/hasher.h"
#include <sstream>

namespace sync {

    Hasher::Hash Hasher::hashBuffer(const std::vector<uint8_t>& data) {
        std::unique_ptr <EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>
        context(EVP_MD_CTX_new(), &EVP_MD_CTX_free);  // memory will be automatically freed when context goes out of scope

        if (!context) {
            throw std::runtime_error("Failed to create EVP context");
        }

        if (EVP_DigestInit_ex(context.get(), EVP_sha256(), nullptr) != 1) {
            throw std::runtime_error("Failed to initialize SHA-256.");
        }

        if (EVP_DigestUpdate(context.get(), data.data(), data.size()) != 1) {
            throw std::runtime_error("Failed to update SHA-256 hash.");
        }

        Hash hash;

        unsigned int length = 0;

        if (EVP_DigestFinal_ex(context.get(), hash.data(), &length) != 1) {
            throw std::runtime_error("Failed to finalize SHA-256 hash.");
        }

        if (length != hash.size()) {
            throw std::runtime_error("Unexpected SHA-256 hash length.");
        }

        return hash;
    }

}