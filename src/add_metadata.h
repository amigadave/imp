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

#ifndef _IMP_ADDMETADATA_H
#define _IMP_ADDMETADATA_H

#include <config.h>
#include <gtkmm.h>
#include <giomm.h>
#include <exiv2/image.hpp>
#include <exiv2/xmp.hpp>
#include <string>

class AddMetadata : public Gtk::Window
{
  public:
    explicit AddMetadata(Glib::RefPtr<Gio::File>& file);
    ~AddMetadata();

  private:
    void on_button_cancel_clicked();
    void on_button_save_clicked();

    Gtk::VBox a_vbox;
    Gtk::Label a_label_header,
      a_label_title,
      a_label_date,
      a_label_creator,
      a_label_owner,
      a_label_rights,
      a_label_publisher,
      a_label_publisher2,
      a_label_identifier,
      a_label_source,
      a_label_format,
      a_label_subject,
      a_label_description,
      a_label_contributors;
    Gtk::Table a_table;
    Gtk::Entry a_entry_title,
      a_entry_date,
      a_entry_identifier,
      a_entry_subject;
    Gtk::ComboBoxEntryText a_cbet_creator,
      a_cbet_owner,
      a_cbet_rights,
      a_cbet_source,
      a_cbet_format;
    Gtk::ScrolledWindow a_scrolledwindow_description,
      a_scrolledwindow_contributors;
    Gtk::TextView a_textview_description,
      a_textview_contributors;
    Gtk::HButtonBox a_buttonbox;
    Gtk::Button a_button_cancel,
      a_button_save;
    Glib::ustring a_file_suffix;
    Glib::RefPtr<Gio::File> a_file;
    Glib::RefPtr<Gtk::TextBuffer> a_textbuffer_description,
      a_textbuffer_contributors;
};

#endif /* _IMP_ADDMETADATA_H */
