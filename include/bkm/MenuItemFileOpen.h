#pragma once

#include <iostream>
#include <bkm/MenuItemBase.h>
#include <bkm/StableCallback.h>
#include <bkm/OpenServiceProvider.h>
#include <bkm/Utils.h>

namespace bkm {

class MenuItemFileOpen : public MenuItemBase {
public:
    MenuItemFileOpen(OpenServiceProvider::cptr_t open_provider, std::string const& path):
        m_callback{this->signal_activate(), [path, o = std::move(open_provider)] {
            std::cout << "activate::MenuItemFileOpen(" << path << ")" << std::endl;
            o->open_file(path);
        }} {
        auto text = bkm::get_file_name(path);
        this->set_base_label_and_tooltip(text);
        this->set_base_image("empty");
    }

private:
    StableCallback m_callback;
};

} // namespace bkm
