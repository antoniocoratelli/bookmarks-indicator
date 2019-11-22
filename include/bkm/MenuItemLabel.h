#pragma once

#include <csignal>
#include <iostream>
#include <gtkmm/menuitem.h>

namespace bkm {

class MenuItemLabel : public Gtk::MenuItem {
public:
    MenuItemLabel(std::string text) {
        this->set_label(text);
        this->set_sensitive(false);
    }

};

} // namespace bkm
