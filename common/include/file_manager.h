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
        /*
         * Constructor for FileManager.
         * @param root The root directory for the file manager.
         */
        explicit FileManager(const std::filesystem::path& root);

        /*
         * Lists all files and directories in the root directory.
         * @return A vector of FileInfo structures representing the files and directories.
         */
        std::vector<FileInfo> listFiles() const;

        /*
         * Checks if a file or directory exists at the specified relative path.
         * @param relativePath The relative path to check.
         * @return True if the file or directory exists, false otherwise.
         */
        bool exists(const std::filesystem::path& relativePath) const;

        /*
         * Creates a directory at the specified relative path.
         * @param relativePath The relative path of the directory to create.
         * @return True if the directory was created successfully, false otherwise.
         */
        bool createDirectory(const std::filesystem::path& relativePath) const;

        /*
         * Removes a file or directory at the specified relative path.
         * @param relativePath The relative path of the file or directory to remove.
         * @return True if the file or directory was removed successfully, false otherwise.
         */
        bool remove(const std::filesystem::path& relativePath) const;

        /*
         * Reads the contents of a file at the specified relative path into a vector of bytes.
         * @param relativePath The relative path of the file to read.
         * @param data The vector to store the file contents in.
         * @return True if the file was read successfully, false otherwise.
         */
        bool readFile(const std::filesystem::path& relativePath, std::vector<uint8_t>& data) const;

        /*
         * Writes data to a file at the specified relative path.
         * @param relativePath The relative path of the file to write.
         * @param data The vector containing the data to write.
         * @return True if the file was written successfully, false otherwise.
         */
        bool writeFile(const std::filesystem::path& relativePath, const std::vector<uint8_t>& data) const;

    private:
        /*
         * Converts a relative path to an absolute path.
         * @param relativePath The relative path to convert.
         * @return The absolute path.
         */
        std::filesystem::path absolutePath(const std::filesystem::path& relativePath) const;
        std::filesystem::path rootDirectory; // The root directory for the file manager
    };

}