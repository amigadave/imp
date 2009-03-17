AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = src/imp
src_imp_SOURCES = src/imp.cc src/main_window.cc src/main_window.h src/add_metadata.cc src/add_metadata.h
src_imp_LDADD = $(GTKMM_LIBS) $(EXIV2_LIBS) $(SQLITE_LIBS)

dist_pkgdata_DATA = data/imp_mainwindow_ui.ui

AM_CPPFLAGS = $(GTKMM_CFLAGS) $(EXIV2_CFLAGS) $(SQLITE_CFLAGS) $(MY_CFLAGS)
DISTCHECK_CONFIGURE_FLAGS = --enable-debug
