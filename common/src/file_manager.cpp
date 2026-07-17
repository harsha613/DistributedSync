#include "../include/file_manager.h"

#include <fstream>

namespace sync {
    FileManager::FileManager(const std::filesystem::path& root)
    : rootDirectory(root) {
        if (!std::filesystem::exists(rootDirectory)) {
            std::filesystem::create_directories(rootDirectory);
        }
    }

    std::filesystem::path FileManager::absolutePath(const std::filesystem::path& relativePath) const {
        return rootDirectory / relativePath;
    }

    bool FileManager::exists(const std::filesystem::path& relativePath) const {
        return std::filesystem::exists(absolutePath(relativePath));
    }

    bool FileManager::createDirectory(const std::filesystem::path& relativePath) const {
        return std::filesystem::create_directories(absolutePath(relativePath));
    }

    bool FileManager::remove(const std::filesystem::path& relativePath) const {
        return std::filesystem::remove(absolutePath(relativePath)) > 0;
    }

    std::vector<FileInfo> FileManager::listFiles() const {

        std::vector<FileInfo> files;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDirectory)) {

            FileInfo info;
            const auto status = entry.status();

            info.relativePath = std::filesystem::relative(entry.path(), rootDirectory);
            info.isDirectory = std::filesystem::is_directory(status);
            info.lastModified = std::filesystem::last_write_time(entry.path());

            if(std::filesystem::is_regular_file(status)) {
                info.size = entry.file_size();
            } else {
                info.size = 0; // For directories, size is set to 0
            }

            files.push_back(info);
        }

        return files;
    }

    bool FileManager::readFile(const std::filesystem::path& relativePath, std::vector<uint8_t>& data) const {

        auto path = absolutePath(relativePath);

        std::ifstream file(path, std::ios::binary);

        if (!file) {
            return false;
        }

        std::uintmax_t size = std::filesystem::file_size(path);

        data.resize(static_cast<std::size_t>(size));

        if (size > 0) {
            file.read(reinterpret_cast<char*>(data.data()), size);
            if (!file) {
                return false; // Read error
            }
        }
        return true;
    }

    bool FileManager::writeFile(const std::filesystem::path& relativePath, const std::vector<uint8_t>& data) const {
        auto path = absolutePath(relativePath);

        std::filesystem::create_directories(path.parent_path()); // Ensure the parent directory exists

        std::ofstream file(path, std::ios::binary);

        if(!file) {
            return false;
        }

        if(!data.empty()) {

            file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
            if(!file) {
                return false; // write error
            }

        }
        return true;
    }
}