#pragma once

#include <csignal>
#include <iostream>
#include <gtkmm/menuitem.h>
#include <bkm/StableCallback.h>

namespace bkm {

class MenuItemQuit : public Gtk::MenuItem {
public:
    MenuItemQuit():
        m_callback{this->signal_activate(), [] {
            std::cout << "activate::MenuItemQuit" << std::endl;
            ::raise(SIGINT);
        }} {
        this->set_label("Quit");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
