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

/** \file add_metadata.cpp
 * Class for adding metadata to images.
 */

#include <config.h>
#include "add_metadata.h"

/** Class for adding metadata to images.
*/
AddMetadata::AddMetadata(Glib::RefPtr<Gio::File>& file) :
  a_vbox(),
  a_label_header("<b>Dublin Core Entities</b>"),
  a_label_title("Title:"),
  a_label_date("Date:"),
  a_label_creator("Creator:"),
  a_label_owner("Owner:"),
  a_label_rights("Rights:"),
  a_label_publisher("Publisher:"),
  a_label_publisher2("Archives, Library and Learning Centre, University of Bath"),
  a_label_identifier("Identifier:"),
  a_label_source("Source:"),
  a_label_format("Physical format:"),
  a_label_subject("Keywords:"),
  a_label_description("Description:"),
  a_label_contributors("Contributors:"),
  a_table(11,3,false),
  a_entry_title(),
  a_entry_date(),
  a_entry_identifier(),
  a_entry_subject(),
  a_cbet_creator(),
  a_cbet_owner(),
  a_cbet_rights(),
  a_cbet_source(),
  a_cbet_format(),
  a_scrolledwindow_description(),
  a_scrolledwindow_contributors(),
  a_textview_description(),
  a_textview_contributors(),
  a_buttonbox(),
  a_button_cancel(Gtk::Stock::CANCEL),
  a_button_save(Gtk::Stock::SAVE)
{
  set_title("Add metadata");
  set_border_width(12);
  set_default_size(400, 300);
  a_label_header.set_use_markup(true);

  /* Set tooltips for entry fields */
  a_entry_title.set_tooltip_text("Title of the image");
  a_entry_date.set_tooltip_text("The date of image creation");
  a_cbet_creator.set_tooltip_text("Creator of the image");
  a_cbet_owner.set_tooltip_text("Copyright owner of the image");
  a_cbet_rights.set_tooltip_text("Copyright terms of the image");
  a_entry_identifier.set_tooltip_text("Unique identifier of the image");
  a_cbet_source.set_tooltip_text("Unique identifier of the source from which the image was derived");
  a_cbet_format.set_tooltip_text("MIME type of the image");
  a_entry_subject.set_tooltip_text("Keywords of the image contents");
  a_textview_description.set_tooltip_text("Descriptive text of the contents of the image");
  a_textview_contributors.set_tooltip_text("List of contributors to the image");

  // ComboBoxEntryText default items, should be read from configuration file or database
  a_cbet_creator.append_text("University Photographer, University of Bath");
  a_cbet_creator.append_text("Nic Delves-Broughton, University Photographer, University of Bath");
  a_cbet_creator.append_text("Tony Holbrook");
  a_cbet_creator.append_text("unknown");
  a_cbet_owner.append_text("University of Bath");
  a_cbet_owner.append_text("Tony Holbrook");
  a_cbet_owner.append_text("unknown");
  a_cbet_rights.append_text("Copyright");
  a_cbet_rights.append_text("CC-BY");
  a_cbet_rights.append_text("CC-BY-NC");
  a_cbet_rights.append_text("CC-BY-NC-ND");
  a_cbet_rights.append_text("CC-BY-NC-SA");
  a_cbet_rights.append_text("CC-BY-ND");
  a_cbet_rights.append_text("CC-BY-SA");
  a_cbet_source.append_text("University Photographic Collection, Archives, Library and Learning Centre, University of Bath");
  a_cbet_source.append_text("Nic Delves-Broughton, University Photographer, University of Bath");
  a_cbet_source.append_text("Department of Estates, University of Bath");
  a_cbet_source.append_text("Tony Holbrook");
  a_cbet_format.append_text("FP4 negative monochrome 24x36 mm film");
  a_cbet_format.append_text("FP4 negative monochrome 6x6 cm film");	
  a_cbet_format.append_text("FP4 negative monochrome 6x4.5 cm film");
  a_cbet_format.append_text("HP5 negative monochrome 24x36 mm film");
  a_cbet_format.append_text("positive colour 24x36 mm film");
  a_cbet_format.append_text("negative colour 24x36 mm film");
  a_cbet_format.append_text("negative monochrome 24x36 mm film");

  // Setup textbuffers for textviews, add to scrolledwindows
  a_textbuffer_description = Gtk::TextBuffer::create();
  a_textbuffer_contributors = Gtk::TextBuffer::create();
  a_textview_description.set_buffer(a_textbuffer_description);
  a_textview_contributors.set_buffer(a_textbuffer_contributors);
  a_scrolledwindow_description.add(a_textview_description);
  a_scrolledwindow_contributors.add(a_textview_contributors);
  a_scrolledwindow_description.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  a_scrolledwindow_contributors.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  a_scrolledwindow_description.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  a_scrolledwindow_contributors.set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  // Connect signal handlers
  a_button_cancel.signal_clicked().connect(sigc::mem_fun(*this,
        &AddMetadata::on_button_cancel_clicked));
  a_button_save.signal_clicked().connect(sigc::mem_fun(*this,
        &AddMetadata::on_button_save_clicked));

  // Fill table, leave first column empty for 12-pixel padding
  a_table.set_row_spacings(6);
  a_table.set_col_spacings(12);
  a_table.attach(a_label_title, 1, 2, 0, 1);
  a_table.attach(a_entry_title, 2, 3, 0, 1);
  a_table.attach(a_label_date, 1, 2, 1, 2);
  a_table.attach(a_entry_date, 2, 3, 1, 2);
  a_table.attach(a_label_creator, 1, 2, 2, 3);
  a_table.attach(a_cbet_creator, 2, 3, 2, 3);
  a_table.attach(a_label_owner, 1, 2, 3, 4);
  a_table.attach(a_cbet_owner, 2, 3, 3, 4);
  a_table.attach(a_label_rights, 1, 2, 4, 5);
  a_table.attach(a_cbet_rights, 2, 3, 4, 5);
  a_table.attach(a_label_publisher, 1, 2, 5, 6);
  a_table.attach(a_label_publisher2, 2, 3, 5, 6);
  a_table.attach(a_label_identifier, 1, 2, 6, 7);
  a_table.attach(a_entry_identifier, 2, 3, 6, 7);
  a_table.attach(a_label_source, 1, 2, 7, 8);
  a_table.attach(a_cbet_source, 2, 3, 7, 8);
  a_table.attach(a_label_format, 1, 2, 8, 9);
  a_table.attach(a_cbet_format, 2, 3, 8, 9);
  a_table.attach(a_label_subject, 1, 2, 9, 10);
  a_table.attach(a_entry_subject, 2, 3, 9, 10);
  a_table.attach(a_label_description, 1, 2, 10, 11);
  a_table.attach(a_scrolledwindow_description, 2, 3, 10, 11);
  a_table.attach(a_label_contributors, 1, 2, 11, 12);
  a_table.attach(a_scrolledwindow_contributors, 2, 3, 11, 12);

  // Buttons at bottom of dialog
  a_buttonbox.set_layout(Gtk::BUTTONBOX_END);
  a_buttonbox.set_spacing(6);
  a_buttonbox.add(a_button_cancel);
  a_buttonbox.add(a_button_save);

  // Pack vbox
  a_vbox.set_spacing(12);
  a_vbox.pack_start(a_label_header, Gtk::PACK_SHRINK);
  a_vbox.pack_start(a_table, Gtk::PACK_SHRINK);
  a_vbox.pack_start(a_buttonbox, Gtk::PACK_SHRINK);
  add(a_vbox);

  // Left-justified labels
  a_label_header.set_alignment(0.0, 0.5);
  a_label_title.set_alignment(0.0, 0.5);
  a_label_date.set_alignment(0.0, 0.5);
  a_label_creator.set_alignment(0.0, 0.5);
  a_label_owner.set_alignment(0.0, 0.5);
  a_label_rights.set_alignment(0.0, 0.5);
  a_label_publisher.set_alignment(0.0, 0.5);
  a_label_publisher2.set_alignment(0.0, 0.5);
  a_label_identifier.set_alignment(0.0, 0.5);
  a_label_source.set_alignment(0.0, 0.5);
  a_label_format.set_alignment(0.0, 0.5);
  a_label_subject.set_alignment(0.0, 0.5);
  a_label_description.set_alignment(0.0, 0.5);
  a_label_contributors.set_alignment(0.0, 0.5);

  // Open image with Exiv2
  a_file = file;
  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(a_file->get_path());
  image->readMetadata();
  Exiv2::XmpData &xmpdata = image->xmpData();

  // Get file name, prefix and suffix
  Glib::ustring file_name = Glib::ustring(a_file->get_basename());
  set_title("Add metadata: " + file_name);
  int dot_position = file_name.find_last_of(".", file_name.size());
  a_file_suffix = file_name.substr(dot_position);
  file_name = file_name.erase(dot_position);

  if(xmpdata.empty())
  {
    // No XMP data found, set title to filename
    a_entry_title.set_text(file_name);
  }
  else
  {
    // Set a_entry_title
    Exiv2::XmpKey key = Exiv2::XmpKey("Xmp.dc.title");
    Exiv2::XmpData::iterator pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
      // Key not found, set title entry to filename
      a_entry_title.set_text(file_name);
    }
    else
    {
      // Key found, use metadata in file
      // chop off file extension in title tag
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.title"];
      Glib::ustring title = Glib::ustring (tag.toString(0));
      int dot_pos = title.find_last_of(".", title.size());
      title = title.erase(dot_pos);
      a_entry_title.set_text(title);
    }

    // Set a_entry_date
    key = Exiv2::XmpKey("Xmp.dc.date");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
      // Key not found, leave date entry blank
      /* TODO: attempt to guess from location in directory tree */
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.date"];
      a_entry_date.set_text(Glib::ustring (tag.toString()));
    }

    // Set a_cbet_creator
    key = Exiv2::XmpKey("Xmp.dc.creator");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.creator"];
      a_cbet_creator.set_active_text(Glib::ustring (tag.toString()));
    }

    // Set a_cbet_owner
    key = Exiv2::XmpKey("Xmp.xmpRights.Owner");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.xmpRights.Owner"];
      a_cbet_owner.set_active_text(Glib::ustring (tag.toString()));
    }

    // Set a_cbet_rights
    key = Exiv2::XmpKey("Xmp.dc.rights");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.rights"];
      a_cbet_rights.set_active_text(Glib::ustring (tag.toString(0)));
    }

    // Set a_entry_identifier
    key = Exiv2::XmpKey("Xmp.dc.identifier");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.identifier"];
      a_entry_identifier.set_text(Glib::ustring (tag.toString()));
    }

    // Set a_cbet_source
    key = Exiv2::XmpKey("Xmp.dc.source");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.source"];
      a_cbet_source.set_active_text(Glib::ustring (tag.toString()));
    }

    //Set a_cbet_format
    key = Exiv2::XmpKey("Xmp.dc.format");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.format"];
      a_cbet_format.set_active_text(Glib::ustring (tag.toString()));
    }

    // Set a_entry_subject
    key = Exiv2::XmpKey("Xmp.dc.subject");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      std::string subject;
      for(long n = 0; n < pos->count(); n++)
      {
        if(n == (pos->count() - 1))
        {
          subject += pos->toString(n);
        }
        else
        {
          subject += pos->toString(n) + ", ";
        }
      }
      a_entry_subject.set_text(Glib::ustring (subject));
    }

    // Set a_textbuffer_description
    key = Exiv2::XmpKey("Xmp.dc.description");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      Exiv2::Xmpdatum& tag = xmpdata["Xmp.dc.description"];
      a_textbuffer_description->set_text(Glib::ustring (tag.toString(0)));
    }

    // Set a_textbuffer_contributors
    key = Exiv2::XmpKey("Xmp.dc.contributors");
    pos = xmpdata.findKey(key);
    if(pos == xmpdata.end())
    {
    }
    else
    {
      std::string contributors;
      for(long n = 0; n < pos->count(); n++)
      {
        if( n == (pos->count() - 1))
        {
          contributors += pos->toString(n);
        }
        else
        {
          contributors += pos->toString(n) + "\n";
        }
      }
      a_textbuffer_contributors->set_text(Glib::ustring (contributors));
    }
  }

  show_all_children();
}

AddMetadata::~AddMetadata()
{
}

/** Hide window on clicking a_button_cancel
*/
void AddMetadata::on_button_cancel_clicked()
{
  this->hide();
}

/** Transfer entry fields to xmpdata object and write metadata to file.
 * Input is validated for each field in turn
 */
void AddMetadata::on_button_save_clicked()
{
  Exiv2::XmpData xmpdata; /**< XmpData object for writing XMP to file */

  /* Xmp.dc.title may only contain alphanumerics and underscores */
  /* TODO: store in language of current locale */
  {
    Glib::ustring test = a_entry_title.get_text();
    test = test.lowercase();
    Glib::ustring::iterator test_iterator = test.begin();
    while(test_iterator != test.end())
    {
      if((Glib::Unicode::isalnum(*test_iterator)) || (*test_iterator == '_'))
      {
        test_iterator++;
      }
      else
      {
        a_entry_title.grab_focus();
        return;
      }
    }
    xmpdata["Xmp.dc.title"] = std::string(test + std::string(a_file_suffix));
  }


  /* Xmp.dc.date must be in ISO 8601 format */
  /* TODO: add time functionality */
  {
    Glib::Date date;
    date.set_parse(a_entry_date.get_text());
    if(date.valid())
    {
      xmpdata["Xmp.dc.date"] = std::string(date.format_string("%F"));
    }
    else
    {
      a_entry_date.grab_focus();
    }
  }

  {
    xmpdata["Xmp.dc.creator"] = std::string(a_cbet_creator.get_active_text());
  }

  {
    xmpdata["Xmp.xmpRights.Owner"] = std::string(a_cbet_owner.get_active_text());
  }

  {
    xmpdata["Xmp.dc.rights"] = std::string(a_cbet_rights.get_active_text());
  }

  /* TODO: Have this set by a config option */
  {
    xmpdata["Xmp.dc.publisher"] = "Archives, Library and Learning Centre, University of Bath";
  }

  {
    xmpdata["Xmp.dc.identifier"] = std::string(a_entry_identifier.get_text());
  }

  {
    xmpdata["Xmp.dc.source"] = std::string(a_cbet_source.get_active_text());
  }

  /* TODO: Use MIME-type and remove UI option */
  {
    xmpdata["Xmp.dc.format"] = std::string(a_cbet_format.get_active_text());
  }

  /* Xmp.dc.subject is an unordered array of "keywords" */
  {
    Glib::ustring keywords = a_entry_subject.get_text();
    keywords.lowercase();
    Exiv2::Value::AutoPtr subject = Exiv2::Value::create(Exiv2::xmpBag);
    if(keywords.find(',', 0) != keywords.npos)
    {
      /* Chop off individual keywords, separated by commas */
      do
      {
        size_t comma_pos = keywords.find_first_of(',', 0);
        Glib::ustring keyword = keywords.substr(0, comma_pos);
        keywords.erase(0, (comma_pos + 1));
        keyword.erase(comma_pos);
        /* Trim spaces for beginning and end of keyword */
        Glib::ustring::iterator nospace = keyword.end();
        nospace--;
        while(Glib::Unicode::isspace(*nospace))
        {
          keyword.erase(nospace);
          nospace--;
        }

        nospace = keyword.begin();
        while(Glib::Unicode::isspace(*nospace))
        {
          keyword.erase(nospace);
        }

        subject->read(std::string(keyword));
      } while(keywords.find(',', 0) != keywords.npos);

      Glib::ustring::iterator nospace = keywords.end();
      nospace--;
      while(Glib::Unicode::isspace(*nospace))
      {
        keywords.erase(nospace);
        nospace--;
      }

      nospace = keywords.begin();
      while(Glib::Unicode::isspace(*nospace))
      {
        keywords.erase(nospace);
      }
      subject->read(std::string(keywords));
    }
    else
    {
      Glib::ustring::iterator nospace = keywords.end();
      nospace--;
      while(Glib::Unicode::isspace(*nospace))
      {
        keywords.erase(nospace);
        nospace--;
      }

      nospace = keywords.begin();
      while(Glib::Unicode::isspace(*nospace))
      {
        keywords.erase(nospace);
      }
      subject->read(std::string(keywords));
    }

    xmpdata.add(Exiv2::XmpKey("Xmp.dc.subject"), subject.get());
  }

  /* Xmp.dc.description is of type LangAlt */
  {
    xmpdata["Xmp.dc.description"] = std::string(a_textbuffer_description->get_text(false));
  }

  /* Xmp.dc.contributors is of type XmpBag */
  {
    Glib::ustring contributors = a_textbuffer_contributors->get_text(false);
    Exiv2::Value::AutoPtr xmpcontributors = Exiv2::Value::create(Exiv2::xmpBag);

    if(contributors.find("\n", 0) != contributors.npos)
    {
      /* Split contributors on newlines */
      /* TODO: swap last name, first name */
      do
      {
        size_t newline_pos = contributors.find_first_of("\n", 0);
        Glib::ustring contributor = contributors.substr(0, newline_pos);
        contributors.erase(0, newline_pos + 1);
        size_t found = contributor.find_first_not_of(" ,.", 0);
        if(found != contributor.npos)
        {
          contributor.erase(0, found);
        }
        found = contributor.find_last_not_of(" ,.", contributor.npos);
        if(found != contributor.npos)
        {
          contributor.erase(found + 1, contributor.npos);
        }

        xmpcontributors->read(std::string(contributor));
      } while(contributors.find("\n", 0) != contributors.npos);
      size_t found = contributors.find_first_not_of(" ,.", 0);
      if(found != contributors.npos)
      {
        contributors.erase(0, found);
      }
      found = contributors.find_last_not_of(" ,.", contributors.npos);
      if(found != contributors.npos)
      {
        contributors.erase(found + 1, contributors.npos);
      }

      xmpcontributors->read(std::string(contributors));
    }
    else
    {
      size_t found = contributors.find_first_not_of(" ,.", 0);
      if(found != contributors.npos)
      {
        contributors.erase(0, found);
      }
      found = contributors.find_last_not_of(" ,.", contributors.npos);
      if(found != contributors.npos)
      {
        contributors.erase(found + 1, contributors.npos);
      }

      xmpcontributors->read(std::string(contributors));
    }

    xmpdata.add(Exiv2::XmpKey("Xmp.dc.contributors"), xmpcontributors.get());

  }

  /* Set internal XMP entities */
  {
    Glib::Date date_now;
    date_now.set_time_current();
    std::string date = date_now.format_string("%F");
    xmpdata["Xmp.xmp.MetadataDate"] = date;
    xmpdata["Xmp.xmp.ModifyDate"] = date;
    xmpdata["Xmp.xmpMM.Manager"] = PACKAGE_STRING;
    //xmpdata["Xmp.xmpMM.ManagerVariant"] = "GTK+ UI";
    xmpdata["Xmp.dc.type"] = "Image";
    xmpdata["Xmp.dc.type"] = "StillImage";
  }

  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(a_file->get_path());
  image->setXmpData(xmpdata);
  /* TODO: allow writing to sidecar .xmp file */
  image->writeMetadata();

  /*if(file_destination->query_exists())
    {
    this->hide();
    return;
    }
    a_file_name.move(file_destination, Gtk::FILE_COPY_OVERWRITE);*/
  /* TODO: add file information to database */
  this->hide();
}
