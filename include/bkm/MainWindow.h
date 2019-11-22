#pragma once

#include <bkm/MenuItemBase.h>

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
    MainWindow() {
        m_icon = Gtk::StatusIcon::create("bookmark-new");
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
