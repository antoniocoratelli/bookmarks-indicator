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

#include <iostream>
#include <gtkmm/menuitem.h>
#include <bkm/OpenServiceProvider.h>
#include <bkm/StableCallback.h>

namespace bkm {

class MenuItemGitHub : public Gtk::MenuItem {
public:
    explicit MenuItemGitHub(OpenServiceProvider::cptr_t osp):
        m_callback{this->signal_activate(), [osp] {
            std::cout << "activate::MenuItemGitHub" << std::endl;
            auto u = "https://github.com/antoniocoratelli/bookmarks-indicator";
            osp->open_url(u);
        }} {
        this->set_label("GitHub");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
