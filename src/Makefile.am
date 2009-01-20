bin_PROGRAMS = imp
imp_SOURCES = imp.cpp main_window.cpp main_window.h add_metadata.cpp add_metadata.h
imp_LDADD = $(GTKMM_LIBS) $(EXIV2_LIBS) $(SQLITE_LIBS)

AM_CPPFLAGS = $(GTKMM_CFLAGS) $(EXIV2_CFLAGS) $(SQLITE_CFLAGS) $(MY_CFLAGS)
EXTRA_DIST = imp_mainwindow_ui.xml
