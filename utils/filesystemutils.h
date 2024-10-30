#include <filesystem>
#include <vector>
#include <string>
#include <cmath>

#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H

enum size_type {B, KB, MB, GB, dynamic};
const int B_MAX = 1024, KB_MAX = std::pow(1024, 2), MB_MAX = pow(1024, 3);

struct size_readable {
    double size;
    enum size_type prefix;
};

class FilesystemUtils {
    static void delete_files(std::filesystem::path directory, std::vector<std::string> file_exceptions);
    static void get_dir_size(std::filesystem::path directory, enum size_type type = dynamic);
    static void delete_dir_recursive(std::filesystem::path directory);
};

#endif // FILESYSTEMUTILS_H
