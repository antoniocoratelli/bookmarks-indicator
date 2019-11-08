#pragma once

#include <iostream>
#include <gtkmm/menuitem.h>
#include <bkm/Config.h>
#include <bkm/StableCallback.h>

namespace bkm {

class MenuItemGitHub : public Gtk::MenuItem {
public:
    explicit MenuItemGitHub(Config::sptr_t config):
        m_callback{this->signal_activate(), [config] {
            std::cout << "activate::MenuItemGitHub TODO" << std::endl;
            auto const url = "https://github.com/antoniocoratelli/bookmarks-indicator";
            std::system(config->get_file_cmd(url).c_str());
        }} {
        this->set_label("GitHub");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
