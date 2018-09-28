#!/usr/bin/env python3
'''
Copyright (c) 2016, Antonio Coratelli.
Released under BSD 3-Clause License. See 'LICENSE' file.

Icon extracted from "WPZOOM Developer Icon Set" by WPZOOM.
http://www.wpzoom.com/wpzoom/new-freebie-wpzoom-developer-icon-set-154-free-icons
'''
import os
import sys
import tempfile
import subprocess

import gi
gi.require_version('Gtk', '3.0')
gi.require_version('AppIndicator3', '0.1')
from gi.repository import Gtk as gtk
from gi.repository import AppIndicator3 as ai
import argparse


class BookmarksIndicator:

    def __init__(self, args):
        self.set_config(args)
        self.init_indicator()
        self.append_menu_items()
        self.append_base_items()
        self.show()

    def show(self):
        gtk.main()

    def restart(self, widget):
        gtk.main_quit()
        self.__init__(self.args)

    def quit(self, widget):
        gtk.main_quit()

    def set_config(self, args):
        '''
        Apply the command-line arguments to the object and parse the
        configuration file.
        '''
        self.args = args
        self.title = "bookmarks-indicator"
        self.label = self.args.label
        self.opener = self.args.opener
        self.icon = self.args.icon
        self.config = self.args.config
        self.shell = self.args.shell
        self.executables = self.args.ext
        with open(self.config) as f:
            # for every line of the configuration file store the path in a list
            # expanding environment variables
            self.folders = [os.path.expandvars(x.strip('\n')) for x in f.readlines()]

    def init_indicator(self):
        '''
        Set the basic properties (icon, label, ...) of the indicator.
        '''
        self.menu = gtk.Menu()
        self.indicator = ai.Indicator.new(self.title, self.icon, ai.IndicatorCategory.APPLICATION_STATUS)
        self.indicator.set_label(self.label, self.label)
        self.indicator.set_status(ai.IndicatorStatus.ACTIVE)
        self.indicator.set_menu(self.menu)

    def append_menu_items(self):
        '''
        Append all the folders specified in the configuration file to the menu.
        '''
        for folder in self.folders:
            if folder == '---':
                self.append_separator(self.menu)
            elif folder.startswith('---'):
                self.append_separator(self.menu, folder[3:].strip())
            elif os.path.isdir(folder):
                self.append_item(self.menu, folder)
            elif os.path.isfile(folder):
                self.append_item(self.menu, folder)
            else:
                raise Exception("'%s' is not a folder or a file" % folder)

    def append_base_items(self):
        '''
        Append Quit, Restart and GitHub link items to the menu.
        '''
        s = gtk.SeparatorMenuItem()
        l = gtk.MenuItem("GitHub")
        r = gtk.MenuItem("Restart")
        q = gtk.MenuItem("Quit")
        l.connect("activate", self.onclick, "https://github.com/antoniocoratelli/bookmarks-indicator")
        r.connect("activate", self.restart)
        q.connect("activate", self.quit)
        for i in [s, l, r, q]: i.show(); self.menu.append(i)

    def append_separator(self, widget, name=None):
        '''
        Append a separator to a menu.
        '''
        sep = gtk.SeparatorMenuItem()
        sep.show()
        widget.append(sep)
        if name != None:
            sep_title = gtk.MenuItem(name)
            sep_title.set_sensitive(False)
            sep_title.show()
            widget.append(sep_title)

    def append_item(self, parent, path):
        '''
        Append a folder or a file to a menu.
        '''
        name = os.path.basename(path)
        items = []
        if name == ".":
            # the item is a '.' element: add a menu entry to open the folder
            # in the default file explorer, and a menu entry to open a shell
            # with that folder as working directory
            item_dot = gtk.MenuItem(".", False)
            item_dot.connect("activate", self.onclick, path)
            item_shell = gtk.MenuItem(">_", False)
            item_shell.connect("activate", self.onshell, path)
            items.append(item_dot)
            items.append(item_shell)
        elif os.path.isdir(path):
            item = gtk.MenuItem(name, False)
            submenu = self.append_dummy(item)
            item.connect("activate", self.onhover, path, submenu)
            items.append(item)
        else:
            item = gtk.MenuItem(name, False)
            item.connect("activate", self.onclick, path)
            items.append(item)
        for i in items:
            i.show()
            parent.append(i)

    def append_dummy(self, widget):
        '''
        Insert a dummy separator in the submenu of a folder item.
        Thanks to this function item representing folders always have an arrow,
        and it is possible to expand the folder content only when the user
        actually hovers the item.
        '''
        submenu = gtk.Menu()
        self.append_separator(submenu)
        widget.set_submenu(submenu)
        return submenu

    def onhover(self, widget, path, submenu):
        '''
        When the user hovers a folder item replace the dummy separator with the
        actual content of the folder.
        '''
        submenu = gtk.Menu()
        widget.set_submenu(submenu)
        self.append_item(submenu, os.path.join(path, "."))
        list_dirs  = get_subdirs(path)
        list_files = get_subfiles(path)
        if len(list_dirs) or len(list_files):
            self.append_separator(submenu)
        for d in sorted(list_dirs):
            self.append_item(submenu, os.path.join(path, d))
        if len(list_dirs) and len(list_files):
            self.append_separator(submenu)
        for f in sorted(list_files):
            self.append_item(submenu, os.path.join(path, f))

    def onclick(self, widget, path):
        '''
        When the user clicks on a file, open it in the default program.
        '''
        if path.endswith(tuple(self.executables)):
            p = subprocess.Popen([path])
        else:
            p = subprocess.Popen([self.opener, path])

    def onshell(self, widget, path):
        '''
        When the user clicks on a '>_' item, open a shell in that folder.
        '''
        p = subprocess.Popen([self.shell], cwd=path)


def get_subdirs(path):
    '''
    Get all the subdirectories of a given folder.
    Output doesn't include hidden subdirectories.
    '''
    out = []
    for f in os.listdir(path):
        if os.path.isdir(os.path.join(path, f))\
        and not f.startswith('.')\
        and not f.endswith('~'):
            out.append(f)
    return out

def get_subfiles(path):
    '''
    Get all the files in a given folder.
    Output doesn't include hidden files.
    '''
    out = []
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f))\
        and not f.startswith('.')\
        and not f.endswith('~'):
            out.append(f)
    return out


if __name__ == "__main__":

    usage = "%s [OPTIONS]" % os.path.basename(__file__)

    # set the default variables for the argment parser
    script_path = os.path.dirname(os.path.realpath(__file__))
    default_config = os.path.join(script_path, "config")
    default_label = ""
    default_opener = "xdg-open"
    default_shell = "gnome-terminal"
    default_color = "#010101"

    # initialize the argument parser object
    parser = argparse.ArgumentParser(usage=usage, epilog=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("-i", action="store", dest="color",  default=default_color,  help="icon color (default: '%s'); accepts all HTML color values, such as '#FF0000' or 'red'; when using hex colors make sure to quote them, otherwise shell bash will interpret everything following # as a comment" % default_color)
    parser.add_argument("-c", action="store", dest="config", default=default_config, help="config file path (default: '%s')" % default_config)
    parser.add_argument("-l", action="store", dest="label",  default=default_label,  help="indicator label (default: '%s')" % default_label)
    parser.add_argument("-o", action="store", dest="opener", default=default_opener, help="file opener (default: '%s')" % default_opener)
    parser.add_argument("-t", action="store", dest="shell",  default=default_shell,  help="shell (default: '%s')" % default_shell)
    parser.add_argument("-x", action="append", dest="ext",   default=[],             help="execute -- instead of open in editor -- files with chosen extension (default: disabled) for example '-x \".sh\" -x \".py\"' will execute bash and python scripts (ensure they have file execution permission)")

    args = parser.parse_args()

    # set the icon color as requested
    indicator_icon = '''<?xml version="1.0" ?><!DOCTYPE svg  PUBLIC '-//W3C//DTD SVG 1.1//EN'  'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'><svg enable-background="new 0 0 500 500" height="20px" width="20px" id="Layer_1" version="1.1" viewBox="0 0 500 500" xml:space="preserve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><path clip-rule="evenodd" d="M68.29,68.29v363.421c0,20.078,16.264,36.34,36.343,36.34h304.363  c12.536,0,22.716-10.175,22.716-22.711c0-12.541-10.18-22.716-22.716-22.716H131.889c-9.996,0-18.17-8.18-18.17-18.172  c0-9.987,8.175-18.168,18.17-18.168h263.478c20.078,0,36.345-16.267,36.345-36.346V68.29c0-20.077-16.267-36.34-36.345-36.34  H268.172v163.45c0,2.36-0.905,4.719-2.636,6.538c-3.538,3.54-9.362,3.54-12.902,0c-2.363-2.457-38.976-32.89-38.976-32.89  s-36.612,30.433-38.977,32.89c-3.54,3.54-9.359,3.54-12.901,0c-1.725-1.819-2.635-4.178-2.635-6.538V31.949h-54.512  C84.553,31.949,68.29,48.213,68.29,68.29z" fill="####" fill-rule="evenodd"/></svg>'''
    indicator_icon = indicator_icon.replace("####", str(args.color))

    # save the icon to a temporary file and start the indicator
    with tempfile.NamedTemporaryFile(suffix='.svg') as icon:
        icon.write(indicator_icon.encode('utf-8'))
        icon.flush()
        args.icon = icon.name
        try:
            bi = BookmarksIndicator(args)
        except Exception as e:
            message = "Error:\n\n%s\n\nDo you want to edit the config file?" % str(e)
            print(message)
            mbox = gtk.MessageDialog(type=gtk.MessageType.ERROR, buttons=gtk.ButtonsType.YES_NO)
            mbox.set_title("bookmarks-indicator")
            mbox.set_markup(message)
            if mbox.run() == gtk.ResponseType.YES:
                print("Opening the configuration file ...")
                p = subprocess.Popen([args.opener, args.config])
            print("Will now quit.")
