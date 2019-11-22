#pragma once

#include <iostream>
#include <bkm/MenuItemBase.h>
#include <bkm/OpenServiceProvider.h>
#include <bkm/StableCallback.h>

namespace bkm {

class MenuItemFolderOpenInShell : public MenuItemBase {
public:
    MenuItemFolderOpenInShell(OpenServiceProvider::cptr_t open_provider, std::string path):
        m_callback{this->signal_activate(), [path, open_provider] {
            std::cout << "activate::MenuItemFolderOpenInShell(" << path << ")" << std::endl;
            open_provider->open_folder_in_terminal(path);
        }} {
        this->set_base_image("utilities-terminal");
        this->set_label("Open Folder in Terminal");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm