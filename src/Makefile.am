INCLUDES =						\
	-DG_LOG_DOMAIN=\"Nautilus-Edit-With-Vim\"	\
	-DDATADIR=\"$(datadir)\"			\
	-DGNOMELOCALEDIR=\""$(datadir)/locale"\" 	\
	-I$(top_srcdir)					\
	-I$(top_builddir)				\
	$(WARN_CFLAGS)                                  \
	-Werror                                         \
	$(DISABLE_DEPRECATED_CFLAGS)			\
	$(NAUTILUS_CFLAGS)				\
	$(GCONF_CFLAGS)					\
	$(GNOMEDESKTOP_CFLAGS)

nautilus_extensiondir=$(NAUTILUS_EXTENSION_DIR)

nautilus_extension_LTLIBRARIES=libnautilus-edit-with-vim.la

libnautilus_edit_with_vim_la_SOURCES = \
	eel-gnome-extensions.c       \
	eel-gnome-extensions.h       \
	nautilus-edit-with-vim.h       \
	nautilus-edit-with-vim.c       \
	nautilus-edit-with-vim.h       \
	edit-with-vim.c

libnautilus_edit_with_vim_la_LDFLAGS = -module -avoid-version
libnautilus_edit_with_vim_la_LIBADD  = $(NAUTILUS_LIBS) $(GCONF_LIBS) $(GNOMEDESKTOP_LIBS)
