#!/usr/bin/env python

import os
import sys
import signal

import gtk
import appindicator as ai

import subprocess

class BookmarksIndicator:

    def __init__(self):
        self.get_config()
        self.init_indicator()
        self.append_menu_items()
        self.append_base_items()
        self.show()

    def show(self):
        gtk.main()

    def restart(self, widget):
        gtk.main_quit();
        self.__init__()
    
    def quit(self, widget):
        sys.exit(0)
    
    def get_config(self):
        self.title = "BookmarksIndicator"
        self.label = ""
        self.opener = "xdg-open"
        self.path = os.path.dirname(os.path.realpath(__file__))
        self.icon = self.path + "/icons/1467483987_78.svg"
        self.config = self.path + "/config"
        with open(self.config) as f:
            self.folders = [os.path.expandvars(x.strip('\n')) for x in f.readlines()]
        
    def init_indicator(self):
        self.menu = gtk.Menu()
        self.indicator = ai.Indicator(self.title, self.icon, ai.CATEGORY_APPLICATION_STATUS)
        self.indicator.set_label(self.label)
        self.indicator.set_status(ai.STATUS_ACTIVE)
        self.indicator.set_menu(self.menu)
    
    def append_menu_items(self):
        for folder in self.folders:
            assert os.path.isdir(folder), "'%s' is not a folder" % folder
            self.append_item(self.menu, folder)
    
    def append_base_items(self):
        s = gtk.SeparatorMenuItem()
        q = gtk.MenuItem("Quit")
        r = gtk.MenuItem("Restart")
        r.connect("activate", self.restart)
        q.connect("activate", self.quit)
        for i in [s, r, q]: i.show(); self.menu.append(i)

    def append_separator(self, widget):
        sep = gtk.SeparatorMenuItem()
        sep.show()
        widget.append(sep)
	    
    def append_item(self, parent, path):
        name = os.path.basename(path)
        item = gtk.MenuItem(name, False)
        if os.path.isdir(path) and name != ".":
            submenu = self.append_dummy(item)
            item.connect("activate", self.onhover, path, submenu)
        else:
            item.connect("activate", self.onclick, path)
        item.show()
        parent.append(item)
    
    def append_dummy(self, widget):
        submenu = gtk.Menu()
        self.append_separator(submenu)
    	widget.set_submenu(submenu)
    	return submenu
	
    def onhover(self, widget, path, submenu):
        submenu = gtk.Menu()
    	widget.set_submenu(submenu)
        self.append_item(submenu, os.path.join(path, "."))
        list_dirs  = get_subdirs(path)
        list_files = get_subfiles(path)
        if len(list_dirs) or len(list_files): self.append_separator(submenu)
        for d in sorted(list_dirs): self.append_item(submenu, os.path.join(path, d))
        if len(list_dirs) and len(list_files): self.append_separator(submenu)
        for f in sorted(list_files): self.append_item(submenu, os.path.join(path, f))

    def onclick(self, widget, path):
        p = subprocess.Popen([self.opener, path])


def get_subdirs(path):
    out = []
    for f in os.listdir(path):
        if os.path.isdir(os.path.join(path, f))\
        and not f.startswith('.')\
        and not f.endswith('~'):
            out.append(f)
    return out

def get_subfiles(path):
    out = []
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f))\
        and not f.startswith('.')\
        and not f.endswith('~'):
            out.append(f)
    return out


if __name__ == "__main__": i = BookmarksIndicator()

