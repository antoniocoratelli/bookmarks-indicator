#pragma once

#include <gtkmm/image.h>
#include <gtkmm/imagemenuitem.h>

namespace bkm {

class MenuItemBase : public Gtk::ImageMenuItem {
public:
    MenuItemBase() = default;

    void set_base_label_and_tooltip(std::string const& text) {
        this->set_tooltip_text(text);
        auto constexpr size_max = 50;
        auto constexpr size_beg = 20;
        auto constexpr size_end = 20;
        auto const size = text.size();
        if (size < size_max) {
            this->set_label(text);
        } else {
            auto s_beg = text.substr(0, size_beg);
            auto s_end = text.substr(size - size_end, size);
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
};

} // namespace bkm
