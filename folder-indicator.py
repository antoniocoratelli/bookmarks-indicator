#!/usr/bin/env python
import sys
import gtk
import appindicator
import os

import threading
import time

from subprocess import call

from os import walk
from os import listdir
from os.path import isfile, isdir, join

from Crypto.Random import random

from xml.dom.minidom import parse
import xml.dom.minidom

import gobject

import re

APP_NAME =  "folder-indicator"
APP_LABEL = "Folder"

DEF_EDITOR = "gedit"

UPDATE_FREQ = 5 # in seconds
PATH_ICON = 'folder-symbolic' #__file__+'-icon.svg'
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
			self.path = c.getAttribute("path")
			
		if not isdir(self.path):
			print "supplied path is not a dir"

		self.refresh_menu()
		
		self.dummy_update()

	def refresh_menu(self):
	
		# initialize empty menu
		self.menu = gtk.Menu()
		
		# get dirs and files list
		list_dirs  = get_subdirs(self.path)
		list_files = get_subfiles(self.path)
		
    	# add dir menu items
		for d in sorted(list_dirs):
			self.append_dir(self.path, d, self.menu)

		# add separator
		if len(list_dirs):
			self.append_separator(self.menu)

		# add file menu items
		for f in sorted(list_files):
			self.append_file(self.path, d, self.menu)

		# add separator
		if len(list_files):
			self.append_separator(self.menu)

		# add restart menu item
		restart_item = gtk.MenuItem("Restart", False)
		restart_item.connect("activate", self.restart)
		restart_item.show()
		self.menu.append(restart_item)

		# add quit menu item
		quit_item = gtk.MenuItem("Quit", False)
		quit_item.connect("activate", self.quit)
		quit_item.show()
		self.menu.append(quit_item)

		# link menu to indicator
		self.ind.set_menu(self.menu)
		
	def dummy_update(self):
		# set callback
		threading.Timer(UPDATE_FREQ, self.dummy_update).start()
		
	#### MENU APPEND FUNCTIONS #################################################
	
	def append_dir(self, path, d, father):
		d_item = gtk.MenuItem(strcut(d,60), False)
		d_item.set_tooltip_text(path+"/"+d)
		d_item.connect("activate", self.item_expand)
		d_item.show()
		father.append(d_item)
		# add dummy separator
		menu_sub = gtk.Menu()
		self.append_separator(menu_sub)
		d_item.set_submenu(menu_sub)
		
	def append_file(self, path, f, father):
		f_item = gtk.MenuItem(strcut(f,60), False)
		f_item.set_tooltip_text(path+"/"+f)
		f_item.connect("activate", self.item_xdgopen)
		f_item.show()
		father.append(f_item)
		
	def append_separator(self, father):
		separator = gtk.SeparatorMenuItem()
		separator.show()
		father.append(separator)
		
	############################################################################
	
	def item_expand(self, widget):

		path = widget.get_tooltip_text()
		sub = gtk.Menu()

		# get dirs and files list
		list_dirs  = get_subdirs(path)
		list_files = get_subfiles(path)
		
		# add dir menu items
		for d in sorted(list_dirs):
			self.append_dir(path, d, sub)
		
		# add separator
		if len(list_dirs) and len(list_files):
			self.append_separator(sub)

		# add file menu items
		for f in sorted(list_files):
			self.append_file(path, f, sub)

		widget.set_submenu(sub)
	
	def item_xdgopen(self, widget):
		os.system('xdg-open "'+widget.get_tooltip_text()+'" &')
		
	def quit(self, widget):
		sys.exit(0)
		
	def restart(self, widget):
		sys.exit(2)
		
################################################################################
################################################################################

def strcut(string, maxlen):
	if len(string) < maxlen:
		return string
	else:
		return string[:maxlen].rsplit(' ', 1)[0]+' ...'

def get_subdirs(path):
	return [f for f in listdir(path) if isdir(join(path, f)) and not f.startswith('.') and not f.endswith('~')]

def get_subfiles(path):
	return [f for f in listdir(path) if isfile(join(path, f)) and not f.startswith('.') and not f.endswith('~')]

def gtk_main_thread():
	gobject.threads_init()
	gtk.main()
	
################################################################################
################################################################################

if __name__ == "__main__":

	# start gui thread
	gtk_thread = threading.Thread(target = gtk_main_thread)
	#gtk_thread.setDaemon(True)
	gtk_thread.start()

	# start indicator thread
	indicator = PanelElement()
	
#	gtk.main()
	
