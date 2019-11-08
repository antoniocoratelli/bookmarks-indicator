#pragma once

#include <gtkmm/imagemenuitem.h>

#include <wordexp.h>

#include <fstream>
#include <memory>
#include <regex>
#include <iomanip>

namespace bkm {

class Config {
public:
    using sptr_t = std::shared_ptr<Config>;

    explicit Config(std::string const& path):
        m_icon{"bookmark-new"},
        m_terminal_cmd{"guake --show -n @PATH"},
        m_folder_cmd{"xdg-open @PATH"},
        m_file_cmd{"xdg-open @PATH"} {
        std::ifstream ifs{path};
        for (std::string line; std::getline(ifs, line); /**/) {
            std::string value;
            if (Config::consume_prefix(line, "icon: ", value))
                m_icon = value;
            else if (Config::consume_prefix(line, "terminal_cmd: ", value))
                m_terminal_cmd = value;
            else if (Config::consume_prefix(line, "folder_cmd: ", value))
                m_folder_cmd = value;
            else if (Config::consume_prefix(line, "file_cmd: ", value))
                m_file_cmd = value;
            else if (Config::consume_prefix(line, "folder: ", value))
                m_folders.push_back(Config::expand_env(value));
            else if (Config::consume_prefix(line, "file: ", value))
                m_files.push_back(Config::expand_env(value));
        }
    }

    bool has_files_and_folders() const {
        return not m_folders.empty() and not m_files.empty();
    }

    std::vector<std::string> const& folders() const {
        return m_folders;
    }

    std::vector<std::string> const& files() const {
        return m_files;
    }

    std::string get_terminal_cmd(std::string const& path) const {
        auto const r = std::regex{"@PATH"};
        std::stringstream p; p << std::quoted(path, '\'');
        return std::regex_replace(m_terminal_cmd, r, p.str()) + "&";
    }

    std::string get_folder_cmd(std::string const& path) const {
        auto const r = std::regex{"@PATH"};
        std::stringstream p; p << std::quoted(path, '\'');
        return std::regex_replace(m_folder_cmd, r, p.str()) + "&";
    }

    std::string get_file_cmd(std::string const& path) const {
        auto const r = std::regex{"@PATH"};
        std::stringstream p; p << std::quoted(path, '\'');
        return std::regex_replace(m_file_cmd, r, p.str()) + "&";
    }

private:
    std::string m_icon;
    std::string m_terminal_cmd;
    std::string m_folder_cmd;
    std::string m_file_cmd;
    std::vector<std::string> m_folders;
    std::vector<std::string> m_files;

    static std::string expand_env(std::string const& source) {
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

    static bool consume_prefix(
            std::string const& source,
            std::string const& prefix,
            std::string& target) {
        target.clear();
        if (source.substr(0, prefix.size()) != prefix)
            return false;
        target = source.substr(prefix.size());
        return true;
    }

};

} // namespace bkm
