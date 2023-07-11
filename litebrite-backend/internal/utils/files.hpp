//
// Created by broosegoose on 7/9/23.
//

#ifndef UTILS_FILES_HPP
#define UTILS_FILES_HPP

#include <filesystem>

namespace utils {

    inline bool IsValidDirectory(const std::filesystem::path& path) {
        try {
            if (!std::filesystem::exists(path)) {
                return false;
            }
            return std::filesystem::is_directory(path);
        }
        catch(const std::filesystem::filesystem_error& e) {
            return false;
        }
    }

    enum class Ternary {
        ERR = 0,
        TRUE = 1,
        FALSE = 2
    };

    inline Ternary FileExists(const std::filesystem::path& path) {
        try {
            if (std::filesystem::exists(path)) {
                if (std::filesystem::is_regular_file(path)) {
                    return Ternary::TRUE;
                } else {
                    return Ternary::ERR;
                }
            }
            return Ternary::FALSE;
        } catch(const std::filesystem::filesystem_error& e) {
            return Ternary::ERR;
        }
    }

}

#endif //UTILS_FILES_HPP
