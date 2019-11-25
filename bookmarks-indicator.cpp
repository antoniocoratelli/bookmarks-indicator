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
#include <bkm/MainWindow.h>
#include <bkm/MenuItemFileOpen.h>
#include <bkm/MenuItemFolderContent.h>
#include <bkm/MenuItemGithub.h>
#include <bkm/MenuItemLabel.h>
#include <bkm/MenuItemQuit.h>
#include <bkm/OpenServiceProvider.h>

#include <CLI11.hpp>
#include <gtkmm/application.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

Glib::RefPtr<Gtk::Application> gApp;

static void quit_on_signal(int s) {
    std::cout << __func__ << " " << s << std::endl;
    gApp->quit();
}

int main(int argc, char const* const* argv) try {
    CLI::App app{"bookmarks-indicator\n\nA handy tool to quickly open the files you use regularly.\n\n"};
    auto app_config = std::string{"$HOME/.bookmarks-indicator.cfg"};
    auto app_opener_file = std::string{"xdg-open"};
    auto app_opener_folder_in_explorer = std::string{"xdg-open"};
    auto app_opener_folder_in_terminal = std::string{"gnome-terminal"};
    auto app_icon = std::string{"bookmark-new"};
    app.add_option("-c", app_config,
        "Configuration file path, default: '" + app_config + "'");
    app.add_option("-o", app_opener_file,
        "Command to open regular files, default: '" + app_opener_file + "'");
    app.add_option("-f", app_opener_folder_in_explorer,
        "Command to open folders in explorer, default: '" + app_opener_folder_in_explorer + "'");
    app.add_option("-t", app_opener_folder_in_terminal,
        "Command to open folders in terminal, default: '" + app_opener_folder_in_terminal + "'");
    app.add_option("-i", app_icon,
        "Icon of the indicator, can be a path or a gtk icon identifier, default: '" + app_icon + "'");
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::vector<std::string> config_lines;
    std::ifstream ifs{bkm::expand_env(app_config)};
    for (std::string line; bool{ifs}; std::getline(ifs, line)) {
        line = bkm::trim_spaces(line);
        line = bkm::expand_env(line);
        config_lines.push_back(line);
    }

    auto osp = std::make_shared<bkm::OpenServiceProvider>(
        app_opener_file,
        app_opener_folder_in_explorer,
        app_opener_folder_in_terminal);

    gApp = Gtk::Application::create("com.coratelli.antonio.bookmarks-indicator");
    ::signal(SIGINT, quit_on_signal);
    ::signal(SIGTERM, quit_on_signal);

    bkm::MainWindow main_window(app_icon);
    for (auto const& line : config_lines) {
        if (line == "---") {
            main_window.append(new Gtk::SeparatorMenuItem());
        } else if (line.substr(0, 3) == "---") {
            auto label = bkm::trim_spaces(line.substr(3));
            main_window.append(new Gtk::SeparatorMenuItem());
            main_window.append(new bkm::MenuItemLabel(label));
        } else if (bkm::is_regular_file(line.c_str())) {
            main_window.append(new bkm::MenuItemFileOpen(osp, line));
        } else if (bkm::is_directory(line.c_str())) {
            main_window.append(new bkm::MenuItemFolderContent(osp, line));
        }
    }
    main_window.append(new Gtk::SeparatorMenuItem());
    main_window.append(new bkm::MenuItemGitHub(osp));
    main_window.append(new bkm::MenuItemQuit());
    return gApp->run(main_window);
} catch(...) {
    return -1;
}
