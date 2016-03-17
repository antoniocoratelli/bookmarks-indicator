#!/usr/bin/env python
import sys
import gtk
import appindicator
import os

from subprocess import call

from os import walk
from os import listdir
from os.path import isfile, isdir, join

from Crypto.Random import random

from xml.dom.minidom import parse
import xml.dom.minidom

import re

APP_NAME =  "folder-indicator"
APP_LABEL = "Folder"

DEF_EDITOR = "gedit"

PING_FREQUENCY = 200
PATH_ICON = 'folder' #__file__+'-icon.svg'
PATH_CONF = __file__+'-conf.xml'

class PanelElement:
	def __init__(self):
		self.ind = appindicator.Indicator(
			APP_NAME,
			PATH_ICON,
			appindicator.CATEGORY_APPLICATION_STATUS
		)
		self.ind.set_label(APP_LABEL)
		self.ind.set_status(appindicator.STATUS_ACTIVE)
		self.ind.set_attention_icon("new-messages-red")

		self.menu_setup()
		self.ind.set_menu(self.menu)

	def menu_setup(self):
		DOMTree = xml.dom.minidom.parse(PATH_CONF)
		collection = DOMTree.documentElement
		
		configs = collection.getElementsByTagName("config")

		for c in configs:
			if not c.hasAttribute("title"):
				print "bad config, missing config:title"
				sys.exit(1)

			if not c.hasAttribute("path"):
				print "bad config, missing config:path"
				sys.exit(1)
				
			print c.getAttribute("title")
			print c.getAttribute("path")
			self.ind.set_label(c.getAttribute("title"))
			mypath = c.getAttribute("path")
			
		bookmarks = collection.getElementsByTagName("b")
		
		print "***** files *****"

		self.menu = gtk.Menu()		
		
		self.populate_menu(self.menu, mypath)

		print "***** app buttons *****"

		self.separator = gtk.SeparatorMenuItem()
		self.separator.show()
		self.menu.append(self.separator)

		self.restart_item = gtk.MenuItem("Restart", False)
		self.restart_item.connect("activate", self.restart)
		self.restart_item.show()
		self.menu.append(self.restart_item)

	def populate_menu(self, menu_father, path):
	
		onlydirs  = [f for f in listdir(path) if  isdir(join(path, f))]
		onlyfiles = [f for f in listdir(path) if isfile(join(path, f))]
		print onlydirs
		print onlyfiles
    	
		new_item = gtk.MenuItem(".", False)
		new_item.set_tooltip_text(path)
		new_item.connect("activate", self.runxdg)
		new_item.show()
		menu_father.append(new_item)

		separator = gtk.SeparatorMenuItem()
		separator.show()
		menu_father.append(separator)

		for d in sorted(onlydirs):
			if not d.startswith('.') and not d.endswith('~'):
				new_item = gtk.MenuItem(d, False)
				new_item.set_tooltip_text(path+"/"+d)
				#new_item.connect("activate", self.runxdg)
				new_item.show()
				menu_father.append(new_item)
			
				menu_sub = gtk.Menu()
				self.populate_menu(menu_sub, path+"/"+d)
				new_item.set_submenu(menu_sub)
			
		separator = gtk.SeparatorMenuItem()
		separator.show()
		menu_father.append(separator)

		for f in sorted(onlyfiles):
			if not f.startswith('.') and not f.endswith('~'):
				new_item = gtk.MenuItem(f, False)
				new_item.set_tooltip_text(path+"/"+f)
				new_item.connect("activate", self.runxdg)
				new_item.show()
				menu_father.append(new_item)

	def main(self):
		#self.check_mail()
		#gtk.timeout_add(PING_FREQUENCY, self.check_mail)
		gtk.main()
		
	def runcommand(self, widget):
		print widget.get_tooltip_text()
		os.system(widget.get_tooltip_text())
		
	def runxdg(self, widget):
		print widget.get_tooltip_text()
		os.system("xdg-open \""+widget.get_tooltip_text()+"\" &")
		
	def quit(self, widget):
		sys.exit(0)
		
	def restart(self, widget):
		sys.exit(2)
		
if __name__ == "__main__":
	indicator = PanelElement()
	indicator.main()

