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
#include <bkm/MenuItemFileOpen.h>
#include <bkm/MenuItemFolderOpenInExplorer.h>
#include <bkm/MenuItemFolderOpenInTerminal.h>
#include <bkm/OpenServiceProvider.h>
#include <bkm/StableCallback.h>
#include <bkm/Utils.h>

#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>

#include <iostream>

namespace bkm {

class MenuItemFolderContent : public MenuItemBase {
public:
    MenuItemFolderContent(OpenServiceProvider::cptr_t osp, std::string path):
        m_open_provider{std::move(osp)},
        m_path{path},
        m_open_in_explorer{m_open_provider, m_path},
        m_open_in_terminal{m_open_provider, m_path},
        m_folders{std::make_shared<GroupFolders>()},
        m_callback{this->signal_activate(), [path, folders = m_folders] {
            std::cout << "activate::MenuItemFolderContent(" << path << ")" << std::endl;
            for (auto& f : *folders)
                f.refresh();
        }} {
        m_submenu.append(m_open_in_explorer);
        m_submenu.append(m_open_in_terminal);
        m_submenu.append(m_separator_1);
        this->set_submenu(m_submenu);
        this->set_base_image("folder");
        auto text = bkm::get_file_name(m_path);
        this->set_base_label_and_tooltip(text);
    }

    void refresh() override {
        this->unset_submenu();
        m_folders->clear();
        m_files.clear();
        m_submenu = Gtk::Menu{};
        m_submenu.append(m_open_in_explorer);
        m_submenu.append(m_open_in_terminal);
        m_submenu.append(m_separator_1);
        auto const sub_folders = bkm::get_content_folders(m_path);
        auto const sub_files = bkm::get_content_files(m_path);
        for (auto const& p : sub_folders)
            m_folders->emplace_back(m_open_provider, p);
        for (auto& f : *m_folders)
            m_submenu.append(f);
        if (not sub_folders.empty() and not sub_files.empty())
            m_submenu.append(m_separator_2);
        for (auto const& p : sub_files)
            m_files.emplace_back(m_open_provider, p);
        for (auto& f : m_files)
            m_submenu.append(f);
        m_submenu.show_all();
        this->set_submenu(m_submenu);
    }

private:
    using GroupFolders = std::vector<bkm::MenuItemFolderContent>;
    using GroupFiles = std::vector<bkm::MenuItemFileOpen>;

    bkm::OpenServiceProvider::cptr_t m_open_provider;
    std::string m_path;
    Gtk::Menu m_submenu;
    bkm::MenuItemFolderOpenInExplorer m_open_in_explorer;
    bkm::MenuItemFolderOpenInTerminal m_open_in_terminal;
    Gtk::SeparatorMenuItem m_separator_1;
    std::shared_ptr<GroupFolders> m_folders;
    Gtk::SeparatorMenuItem m_separator_2;
    GroupFiles m_files;

    StableCallback m_callback;
};

} // namespace bkm
