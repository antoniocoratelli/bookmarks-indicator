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

#include <gtkmm/image.h>
#include <gtkmm/imagemenuitem.h>

#include <memory>

namespace bkm {

class MenuItemBase : public Gtk::ImageMenuItem {
public:
    MenuItemBase() = default;

    virtual void refresh() {}

    void set_base_label_and_tooltip(std::string const& text) {
        this->set_tooltip_text(text);
        auto const size = text.size();
        if (size < m_size_max) {
            this->set_label(text);
        } else {
            auto s_beg = text.substr(0, m_size_beg);
            auto s_end = text.substr(size - m_size_end, size);
            this->set_label(s_beg + " ... " + s_end);
        }
    }

    void set_base_image(char const* image_name) {
        auto image_size = Gtk::BuiltinIconSize::ICON_SIZE_MENU;
        m_image.set_from_icon_name(image_name, image_size);
        this->set_image(m_image);
        this->set_always_show_image(true);
    }

private:
    Gtk::Image m_image;
    static auto constexpr m_size_max = 50;
    static auto constexpr m_size_beg = 20;
    static auto constexpr m_size_end = 20;
};

} // namespace bkm
