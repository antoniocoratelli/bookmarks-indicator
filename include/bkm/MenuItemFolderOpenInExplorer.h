#pragma once

#include <iostream>
#include <bkm/MenuItemBase.h>
#include <bkm/OpenServiceProvider.h>
#include <bkm/StableCallback.h>
#include <bkm/Utils.h>

namespace bkm {

class MenuItemFolderOpenInExplorer : public MenuItemBase {
public:
    MenuItemFolderOpenInExplorer(
            OpenServiceProvider::cptr_t open_provider, std::string path):
        m_callback{this->signal_activate(), [path, open_provider] {
            std::cout << "activate::MenuItemFolderOpenInExplorer(" << path << ")" << std::endl;
            open_provider->open_folder_in_explorer(path);
        }} {
        this->set_base_image("system-file-manager");
        this->set_label("Open Folder");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
