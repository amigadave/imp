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

/** \file imp.cpp
 * IMP Metadata Project - A program to catalogue a collection of images.
 */

#include <config.h>
#include "main_window.h"
#include <gtkmm/main.h>

/** Main function.
 * Performs setup of GTK, GIO, etc.
 * \param argc Number of arguments.
 * \param argv Argument string.
 */
int main(int argc, char *argv[])
{
	Gtk::Main kit(argc, argv);

	MainWindow main_window;

	Gtk::Main::run(main_window);

	return 0;
}
