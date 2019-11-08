#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <dirent.h>
#include <unistd.h>

namespace bkm {

std::string get_file_name(std::string const& path) {
    auto pos_sep = path.find_last_of('/');
    auto pos_beg = path.find_first_not_of('/', pos_sep);
    auto idx = (pos_beg == std::string::npos) ? 0 : pos_beg;
    return path.substr(idx);
}

std::vector<std::string> get_content_folders(std::string path) {
    if (not path.empty() and path.back() != '/')
        path.push_back('/');
    std::vector<std::string> result;
    DIR* dir = ::opendir(path.c_str());
    if (dir == nullptr)
        throw std::runtime_error{"Could not list folder " + path};
    for (struct dirent* e = ::readdir(dir); e != nullptr; e = ::readdir(dir)) {
        auto name = std::string{e->d_name};
        if (name.empty() or name.front() == '.' or e->d_type != DT_DIR)
            continue;
        result.push_back(path + name);
    }
    ::closedir(dir);
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<std::string> get_content_files(std::string path) {
    if (not path.empty() and path.back() != '/')
        path.push_back('/');
    std::vector<std::string> result;
    DIR* dir = ::opendir(path.c_str());
    if (dir == nullptr)
        throw std::runtime_error{"Could not list folder " + path};
    for (struct dirent* e = ::readdir(dir); e != nullptr; e = ::readdir(dir)) {
        auto name = std::string{e->d_name};
        if (name.empty() or name.front() == '.' or e->d_type != DT_REG)
            continue;
        result.push_back(path + name);
    } ::closedir(dir);
    std::sort(result.begin(), result.end());
    return result;
}

} // namespace bkm
