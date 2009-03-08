/* IMP is copyright David King and Ekaterina Gerasimova, 2008
 *
 * This file is part of IMP.
 *
 * IMP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IMP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IMP.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file main_window.h
 * Prototypes for main_window.cpp
 */

#ifndef _IMP_MAINWINDOW_H
#define _IMP_MAINWINDOW_H

#include <config.h>
#include <gtkmm.h>
#include <giomm.h>
#include "add_metadata.h"

class MainWindow : public Gtk::Window
{
	public:
		MainWindow();
		~MainWindow();
	
	private:
		void on_file_add();
		void on_file_quit();
		void on_edit_preferences();
		void on_help_about();
		
		virtual Glib::RefPtr<Gio::File>& get_source_directory();
		virtual Glib::RefPtr<Gio::File>& get_destination_directory();

		unsigned int m_context_id,
			     m_count;

		Glib::RefPtr<Gio::File> m_file_config;
		Glib::RefPtr<Gio::File> m_file;
		Glib::KeyFile m_keyfile_config;
		Gtk::VBox m_vbox;
		Glib::RefPtr<Gtk::ActionGroup> m_actiongroup;
		Glib::RefPtr<Gtk::UIManager> m_uimanager;
		Gtk::TreeView m_treeview;
		Gtk::Statusbar m_statusbar;
		Glib::ustring m_config_source_dir,
			      m_config_destination_dir;
};
