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

#include <bkm/MenuItemBase.h>
#include <bkm/Utils.h>

#include <gtkmm/window.h>
#include <gtkmm/statusicon.h>
#include <gtkmm/stock.h>
#include <gtkmm/main.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

namespace bkm {

class MainWindow : public Gtk::Window {
public:
    explicit MainWindow(std::string icon_path_or_id) {
        bool const icon_is_path = bkm::is_regular_file(icon_path_or_id.c_str());
        m_icon = icon_is_path ?
            Gtk::StatusIcon::create_from_file(icon_path_or_id):
            Gtk::StatusIcon::create(icon_path_or_id);
        m_icon->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::callback_activate));
        m_menu.signal_button_release_event().connect(
            sigc::mem_fun(*this, &MainWindow::callback_button_release), false);
        this->set_skip_taskbar_hint(true);
        this->iconify();
    }

    void append(Gtk::MenuItem* new_item) {
        Glib::RefPtr<Gtk::MenuItem> ptr_item{new_item};
        m_items.push_back(std::move(ptr_item));
        m_menu.append(*m_items.back().operator->());
        m_menu.show_all();
    }

    ~MainWindow() override {
        m_icon->set_visible(false);
    }

private:
    void callback_activate() {
        std::cout << "activate::MainWindow" << std::endl;
        for (auto& i : m_items) {
            auto const p = dynamic_cast<bkm::MenuItemBase*>(i.operator->());
            if (p != nullptr)
                p->refresh();
        }
        m_icon->popup_menu_at_position(m_menu, 1, 0);
    }

    bool callback_button_release(GdkEventButton* event) {
        auto const w = ::gtk_widget_get_window(GTK_WIDGET(m_menu.gobj()));
        return (event->window == w);
    }

    Glib::RefPtr<Gtk::StatusIcon> m_icon;
    Gtk::Menu m_menu;
    std::vector<Glib::RefPtr<Gtk::MenuItem>> m_items;
};

} // namespace bkm
