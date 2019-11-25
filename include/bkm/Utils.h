//    Copyright (C) 2019, Antonio Coratelli
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wordexp.h>

namespace bkm {

inline std::string get_file_name(std::string const& path) {
    auto pos_sep = path.find_last_of('/');
    auto pos_beg = path.find_first_not_of('/', pos_sep);
    auto idx = (pos_beg == std::string::npos) ? 0 : pos_beg;
    return path.substr(idx);
}

inline std::string trim_spaces(std::string s) {
    std::reverse(s.begin(), s.end());
    while (not s.empty() and s.back() == ' ')
        s.pop_back();
    std::reverse(s.begin(), s.end());
    while (not s.empty() and s.back() == ' ')
        s.pop_back();
    return s;
}

inline std::vector<std::string> get_content_folders(std::string path) {
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

inline std::vector<std::string> get_content_files(std::string path) {
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

inline std::string expand_env(std::string const& source) {
    ::wordexp_t p;
    char** w;
    if (::wordexp(source.c_str(), &p, 0) != 0)
        throw std::runtime_error{"wordexp failure"};
    w = p.we_wordv;
    std::stringstream result;
    for (size_t i = 0; i < p.we_wordc; ++i)
        result << w[i];
    ::wordfree(&p);
    return result.str();
}

inline bool is_regular_file(const char *path) {
    struct stat path_stat;
    ::stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

inline bool is_directory(const char *path) {
    struct stat path_stat;
    ::stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

} // namespace bkm
