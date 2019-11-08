#pragma once

#include <iostream>
#include <bkm/MenuItemBase.h>
#include <bkm/StableCallback.h>
#include <bkm/Utils.h>

namespace bkm {

class MenuItemFileOpen : public MenuItemBase {
public:
    MenuItemFileOpen(Config::sptr_t config, std::string path):
        m_callback{this->signal_activate(), [path, config] {
            std::cout << "activate::MenuItemFileOpen(" << path << ")" << std::endl;
            std::system(config->get_file_cmd(path).c_str());
        }} {
        auto text = bkm::get_file_name(path);
        this->set_base_label_and_tooltip(text);
        this->set_base_image("empty");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
