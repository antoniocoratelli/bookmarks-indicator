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

#include <memory>
#include <string>
#include <thread>
#include <procxx.h>

namespace bkm {

class OpenServiceProvider {
public:
    using uptr_t = std::unique_ptr<OpenServiceProvider>;
    using sptr_t = std::shared_ptr<OpenServiceProvider>;
    using cptr_t = std::shared_ptr<const OpenServiceProvider>;

    OpenServiceProvider(
            std::string cli_file,
            std::string cli_folder_in_explorer,
            std::string cli_folder_in_terminal) {
        this->set_open_file(cli_file);
        this->set_open_folder_in_explorer(cli_folder_in_explorer);
        this->set_open_folder_in_terminal(cli_folder_in_terminal);
    }

    void open_url(std::string path) const {
        this->open_file(std::move(path));
    }

    void open_file(std::string path) const {
        std::thread{OpenServiceProvider::_execute,
            m_open_file_proc,
            m_open_file_args,
            path}.detach();
    }

    void open_folder_in_explorer(std::string path) const {
        std::thread{OpenServiceProvider::_execute,
            m_open_folder_in_explorer_proc,
            m_open_folder_in_explorer_args,
            path}.detach();
    }

    void open_folder_in_terminal(std::string path) const {
        std::thread{OpenServiceProvider::_execute,
            m_open_folder_in_terminal_proc,
            m_open_folder_in_terminal_args,
            path}.detach();
    }

private:
    std::string m_open_file_proc;
    std::string m_open_folder_in_explorer_proc;
    std::string m_open_folder_in_terminal_proc;
    std::vector<std::string> m_open_file_args;
    std::vector<std::string> m_open_folder_in_explorer_args;
    std::vector<std::string> m_open_folder_in_terminal_args;

    static void _execute(
            std::string exec,
            std::vector<std::string> args,
            std::string path) try {
        procxx::process proc{exec};
        for (auto const& a : args)
            proc.add_argument(a);
        proc.add_argument(path);
        proc.exec();
        proc.wait();
    } catch (...) {
        std::stringstream sst;
        sst << exec << " ";
        for (auto const& a : args)
            sst << a << " ";
        sst << path;
        std::cout << "Error running '" << sst.str() << "'" << std::endl;
    }

    void set_open_file(std::string const& cli) {
        std::istringstream iss(cli);
        iss >> m_open_file_proc;
        while (iss) {
            std::string arg; iss >> arg;
            if (not arg.empty())
                m_open_file_args.push_back(arg);
        }
    }

    void set_open_folder_in_explorer(std::string const& cli) {
        std::istringstream iss(cli);
        iss >> m_open_folder_in_explorer_proc;
        while (iss) {
            std::string arg; iss >> arg;
            if (not arg.empty())
                m_open_folder_in_explorer_args.push_back(arg);
        }
    }

    void set_open_folder_in_terminal(std::string const& cli) {
        std::istringstream iss(cli);
        iss >> m_open_folder_in_terminal_proc;
        while (iss) {
            std::string arg; iss >> arg;
            if (not arg.empty())
                m_open_folder_in_terminal_args.push_back(arg);
        }
    }
};

} // namespace bkm
