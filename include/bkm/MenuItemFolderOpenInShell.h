#pragma once

#include <iostream>
#include <bkm/MenuItemBase.h>
#include <bkm/StableCallback.h>

namespace bkm {

class MenuItemFolderOpenInShell : public MenuItemBase {
public:
    MenuItemFolderOpenInShell(Config::sptr_t config, std::string path):
        m_callback{this->signal_activate(), [path, config] {
            std::cout << "activate::MenuItemFolderOpenInShell(" << path << ")" << std::endl;
            std::system(config->get_terminal_cmd(path).c_str());
        }} {
        this->set_base_image("utilities-terminal");
        this->set_label("Open Folder in Terminal");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm