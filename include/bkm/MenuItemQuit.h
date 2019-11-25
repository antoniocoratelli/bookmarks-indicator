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
