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

/** \file main_window.cpp
 * Class for main window of program.
 */

#include <config.h>
#include "main_window.h"

/* Constructor for main window.
*/
MainWindow::MainWindow() :
  m_count(1),
  m_vbox(false, 0)
{
  set_title(PACKAGE_STRING);
  set_default_size(200, 200);

  // Configuration file
  m_file_config = Gio::File::create_for_path(Glib::get_user_config_dir() + "/imp.conf");
  if(m_file_config->query_exists())
  {
    // Read from config file
    // TODO: Fix ugly chained try, catch.
    m_keyfile_config.load_from_file(m_file_config->get_path());
    try
    {
      m_config_source_dir = m_keyfile_config.get_value("files", "source_directory");
    }
    catch(const Glib::KeyFileError& exception)
    {
      std::cerr << "Exception while reading config file: " << exception.what() << std::endl;
      m_config_source_dir = Glib::get_home_dir();
    }
    try
    {
      m_config_destination_dir = m_keyfile_config.get_value("files", "destination_directory");
    }
    catch(const Glib::KeyFileError& exception)
    {
      std::cerr << "Exception while reading config file: " << exception.what() << std::endl;
      m_config_destination_dir = Glib::get_home_dir();
    }
  }
  else
  {
    // Set sensible defaults
    // TODO: create default config file
    m_config_source_dir = Glib::get_home_dir();
    m_config_destination_dir = Glib::get_home_dir();
  }

  // TODO: Status bar
  m_context_id = m_statusbar.get_context_id(PACKAGE_STRING);

  m_actiongroup = Gtk::ActionGroup::create();
  m_actiongroup->add(Gtk::Action::create("FileMenu", "File"));
  m_actiongroup->add(Gtk::Action::create("FileAdd", Gtk::Stock::ADD),
      sigc::mem_fun(*this, &MainWindow::on_file_add));
  m_actiongroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
      sigc::mem_fun(*this, &MainWindow::on_file_quit));
  m_actiongroup->add(Gtk::Action::create("EditMenu", "Edit"));
  m_actiongroup->add(Gtk::Action::create("EditPreferences", Gtk::Stock::PREFERENCES), sigc::mem_fun(*this, &MainWindow::on_edit_preferences));
  m_actiongroup->add(Gtk::Action::create("HelpMenu", "Help"));
  m_actiongroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT),
      sigc::mem_fun(*this, &MainWindow::on_help_about));

  m_uimanager = Gtk::UIManager::create();
  m_uimanager->insert_action_group(m_actiongroup);
  add_accel_group(m_uimanager->get_accel_group());
  // TODO: Load uimanager file from correct location.
  m_uimanager->add_ui_from_file("imp_mainwindow_ui.xml");
  Gtk::Widget* m_menubar = m_uimanager->get_widget("/m_menubar");
  Gtk::Widget* m_toolbar = m_uimanager->get_widget("/m_toolbar");

  if(m_menubar)
  {
    m_vbox.pack_start(*m_menubar, Gtk::PACK_SHRINK);
  }
  if(m_toolbar)
  {
    m_vbox.pack_start(*m_toolbar, Gtk::PACK_SHRINK);
  }
  m_vbox.pack_start(m_treeview, Gtk::PACK_EXPAND_WIDGET);
  m_vbox.pack_start(m_statusbar, Gtk::PACK_SHRINK);

  add(m_vbox);

  show_all_children();
}

MainWindow::~MainWindow()
{
}

/** Function to respond to "add metadata" signal.
*/
void MainWindow::on_file_add()
{
  Gtk::FileChooserDialog dialog("Select a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
  dialog.set_current_folder(m_config_source_dir);

  Gtk::FileFilter filter_image;
  filter_image.set_name("Image files");
  filter_image.add_mime_type("image/*");
  dialog.add_filter(filter_image);
  Gtk::FileFilter all_files;
  all_files.set_name("All files");
  all_files.add_pattern("*");
  dialog.add_filter(all_files);

  int result = dialog.run();

  switch(result)
  {
    case(Gtk::RESPONSE_OK):
      {
        Glib::RefPtr<Gio::File> file_name = Gio::File::create_for_path(dialog.get_filename());
        AddMetadata addmetadata(file_name);
        dialog.hide();
        Gtk::Main::run(addmetadata);
        break;
      }
    case(Gtk::RESPONSE_CANCEL):
      {
        break;
      }
    default:
      {
        std::cerr << "Invalid button clicked in Add Metadata dialog" << std::endl;
        break;
      }
  }
}

/** Function to quit program.
*/
void MainWindow::on_file_quit()
{
  hide();
}

/** Function for displaying preferences dialog.
*/
void MainWindow::on_edit_preferences()
{
  // TODO: Preferences dialog
}

/** Function for displaying about dialog.
*/
void MainWindow::on_help_about()
{
  Gtk::AboutDialog dialog;
  dialog.set_program_name(PACKAGE_NAME);
  dialog.set_version(PACKAGE_VERSION);
  dialog.set_copyright("Copyright 2008 Ekaterina Gerasimova and David King");
  dialog.set_comments("A project to add metadata to files and index them in a searchable database");
  dialog.set_license("IMP is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\nIMP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with IMP.  If not, see <http://www.gnu.org/licenses/>.");
  dialog.set_wrap_license(true);
  // TODO: additional tabs in help dialog
  //dialog.set_authors("David King\nEkaterina Gerasimova");
  //dialog.set_documenters("Ekaterina Gerasimova");

  /*int result = */dialog.run();
}

/** Function for determination of image source directory.
*/
Glib::RefPtr<Gio::File>& MainWindow::get_source_directory()
{
  m_file = Gio::File::create_for_path(m_config_source_dir);
  return(m_file);
}

/** Function for determination of image destination directory.
*/
Glib::RefPtr<Gio::File>& MainWindow::get_destination_directory()
{
  m_file = Gio::File::create_for_path(m_config_destination_dir);
  return(m_file);
}
