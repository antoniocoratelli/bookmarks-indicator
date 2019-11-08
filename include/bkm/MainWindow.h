#pragma once

#include <bkm/Config.h>
#include <bkm/MenuItemFileOpen.h>
#include <bkm/MenuItemFolderContent.h>
#include <bkm/MenuItemGithub.h>
#include <bkm/MenuItemQuit.h>

#include <gtkmm/window.h>
#include <gtkmm/statusicon.h>
#include <gtkmm/stock.h>
#include <gtkmm/main.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>

#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace bkm {

class MainWindow : public Gtk::Window {
public:
    explicit MainWindow(Config::sptr_t config):
        m_item_github{config} {
        m_folders.reserve(config->folders().size());
        m_files.reserve(config->files().size());
        for (auto const& p : config->folders())
            m_folders.emplace_back(config, p);
        for (auto& i : m_folders)
            m_menu.append(i);
        if (config->has_files_and_folders())
            m_menu.append(m_separator_1);
        for (auto const& p : config->files())
            m_files.emplace_back(config, p);
        for (auto& i : m_files)
            m_menu.append(i);
        m_menu.append(m_separator_2);
        m_menu.append(m_item_github);
        m_menu.append(m_item_quit);
        m_menu.show_all();
        m_icon = Gtk::StatusIcon::create("bookmark-new");
        m_icon->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::callback_activate));
        m_menu.signal_button_release_event().connect(
            sigc::mem_fun(*this, &MainWindow::callback_button_release), false);
        this->set_skip_taskbar_hint(true);
        this->iconify();
    }

    ~MainWindow() override {
        m_icon->set_visible(false);
    }

private:
    void callback_activate() {
        std::cout << "activate::MainWindow" << std::endl;
        for (auto& i : m_folders)
            i.populate();
        m_icon->popup_menu_at_position(m_menu, 1, 0);
    }

    bool callback_button_release(GdkEventButton* event) {
        auto const w = gtk_widget_get_window(GTK_WIDGET(m_menu.gobj()));
        return (event->window == w);
    }

    Glib::RefPtr<Gtk::StatusIcon> m_icon;
    Gtk::Menu m_menu;
    std::vector<bkm::MenuItemFolderContent> m_folders;
    Gtk::SeparatorMenuItem m_separator_1;
    std::vector<bkm::MenuItemFileOpen> m_files;
    Gtk::SeparatorMenuItem m_separator_2;
    bkm::MenuItemGitHub m_item_github;
    bkm::MenuItemQuit m_item_quit;
};

} // namespace bkm
