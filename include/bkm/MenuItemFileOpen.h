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
#include <bkm/MenuItemBase.h>
#include <bkm/StableCallback.h>
#include <bkm/OpenServiceProvider.h>
#include <bkm/Utils.h>

namespace bkm {

class MenuItemFileOpen : public MenuItemBase {
public:
    MenuItemFileOpen(OpenServiceProvider::cptr_t osp, std::string path):
        m_callback{this->signal_activate(), [path, osp] {
            std::cout << "activate::MenuItemFileOpen(" << path << ")" << std::endl;
            osp->open_file(path);
        }} {
        auto text = bkm::get_file_name(path);
        this->set_base_label_and_tooltip(text);
        this->set_base_image("empty");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
