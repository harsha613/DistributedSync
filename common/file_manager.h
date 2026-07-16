#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

namespace sync {

    struct FileInfo {

        std::filesystem::path relativePath; // Relative path of the file
        std::uintmax_t size; // Size of the file in bytes
        std::filesystem::file_time_type lastModified; // Last modified timestamp
        bool isDirectory; // Flag to indicate if it's a directory

    };

    class FileManager {
    public:

        explicit FileManager(const std::filesystem::path& root);

        std::vector<FileInfo> listFiles() const;

        bool exists(const std::filesystem::path& relativePath) const;

        bool createDirectory(const std::filesystem::path& relativePath);

        bool deleteFile(const std::filesystem::path& relativePath);

        bool readFile(const std::filesystem::path& relativePath, std::vector<uint8_t>& data) const;

        bool writeFile(const std::filesystem::path& relativePath, const std::vector<uint8_t>& data);

    private:
        std::filesystem::path rootDirectory;
    };

}