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
#include <bkm/OpenServiceProvider.h>
#include <bkm/StableCallback.h>

namespace bkm {

class MenuItemFolderOpenInExplorer : public MenuItemBase {
public:
    MenuItemFolderOpenInExplorer(OpenServiceProvider::cptr_t osp, std::string path):
        m_callback{this->signal_activate(), [path, osp] {
            std::cout << "activate::MenuItemFolderOpenInExplorer(" << path << ")" << std::endl;
            osp->open_folder_in_explorer(path);
        }} {
        this->set_base_image("system-file-manager");
        this->set_label("Open Folder");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
