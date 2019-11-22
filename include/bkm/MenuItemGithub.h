#pragma once

#include <iostream>
#include <gtkmm/menuitem.h>
#include <bkm/OpenServiceProvider.h>
#include <bkm/StableCallback.h>

namespace bkm {

class MenuItemGitHub : public Gtk::MenuItem {
public:
    explicit MenuItemGitHub(OpenServiceProvider::cptr_t open_provider):
        m_callback{this->signal_activate(), [open_provider] {
            std::cout << "activate::MenuItemGitHub" << std::endl;
            auto u = "https://github.com/antoniocoratelli/bookmarks-indicator";
            open_provider->open_url(u);
        }} {
        this->set_label("GitHub");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
