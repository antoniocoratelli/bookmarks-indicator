#pragma once

#include <iostream>
#include <bkm/MenuItemBase.h>
#include <bkm/StableCallback.h>
#include <bkm/Utils.h>

namespace bkm {

class MenuItemFolderOpenInExplorer : public MenuItemBase {
public:
    MenuItemFolderOpenInExplorer(Config::sptr_t config, std::string path):
        m_callback{this->signal_activate(), [path, config] {
            std::cout << "activate::MenuItemFolderOpenInExplorer(" << path << ")" << std::endl;
            std::system(config->get_folder_cmd(path).c_str());
        }} {
        this->set_base_image("system-file-manager");
        this->set_label("Open Folder");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
