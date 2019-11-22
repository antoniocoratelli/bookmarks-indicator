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
    app.add_option("-c", app_config,
        "Configuration file path, default: '" + app_config + "'");
    app.add_option("-o", app_opener_file,
        "Command to open regular files, default: '" + app_opener_file + "'");
    app.add_option("-f", app_opener_folder_in_explorer,
        "Command to open folders in explorer, default: '" + app_opener_folder_in_explorer + "'");
    app.add_option("-t", app_opener_folder_in_terminal,
        "Command to open folders in terminal, default: '" + app_opener_folder_in_terminal + "'");
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::vector<std::string> config_lines;
    std::ifstream ifs{bkm::expand_env(app_config)};
    for (std::string line; bool{ifs}; std::getline(ifs, line))
        config_lines.push_back(bkm::expand_env(line));

    auto osp = std::make_shared<bkm::OpenServiceProvider>(
        app_opener_file,
        app_opener_folder_in_explorer,
        app_opener_folder_in_terminal);

    gApp = Gtk::Application::create("com.coratelli.antonio.bookmarks-indicator");
    ::signal(SIGINT, quit_on_signal);
    ::signal(SIGTERM, quit_on_signal);

    bkm::MainWindow main_window;
    for (auto const& line : config_lines) {
        if (line == "---") {
            main_window.append(new Gtk::SeparatorMenuItem());
        } else if (line.substr(0, 3) == "---") {
            main_window.append(new Gtk::SeparatorMenuItem());
            main_window.append(new bkm::MenuItemLabel(line.substr(3)));
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
