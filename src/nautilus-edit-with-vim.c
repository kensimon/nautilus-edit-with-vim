/*
 *  nautilus-edit-with-vim.c
 * 
 *  Copyright (C) 2010 Ken Simon
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Author: Ken Simon <ken@ninkendo.org>
 * 
 */

#if HAVE_CONFIG_H
#include <config.h> /* for GETTEXT_PACKAGE */
#endif

#include "nautilus-edit-with-vim.h"

#include <libnautilus-extension/nautilus-menu-provider.h>

#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <libgnome/gnome-desktop-item.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h> /* for atoi */
#include <string.h> /* for strcmp */
#include <unistd.h> /* for chdir */
#include <sys/stat.h>

static void nautilus_edit_with_vim_instance_init (NautilusEditWithVim      *cvs);
static void nautilus_edit_with_vim_class_init    (NautilusEditWithVimClass *class);

static GType editwithvim_type = 0;

typedef enum {
	/* local files. Always open "conventionally", i.e. cd and spawn. */
	FILE_INFO_LOCAL,
	FILE_INFO_DESKTOP,
	/* SFTP: Shell terminals are opened "remote" (i.e. with ssh client),
	 * commands are executed like *_OTHER */
	FILE_INFO_SFTP,
	/* OTHER: Terminals and commands are opened by mapping the URI back
	 * to ~/.gvfs, i.e. to the GVFS FUSE bridge
	 */
	FILE_INFO_OTHER
} EditWithVimFileInfo;

static inline char *
get_gvfs_path_for_uri (const char *uri)
{
	GFile *file;
	char *path;

	file = g_file_new_for_uri (uri);
	path = g_file_get_path (file);
	g_object_unref (file);

	return path;
}

static void
edit_with_vim (NautilusMenuItem *item,
	       NautilusFileInfo *file_info)
{
	char* path = g_file_get_path (nautilus_file_info_get_location (file_info));
	char* pwd = g_file_get_path (nautilus_file_info_get_parent_location (file_info));
	GdkScreen* screen = g_object_get_data (G_OBJECT (item), "NautilusEditWithVim::screen");
	char* argv[3];
	argv[0] = "gvim";
	argv[1] = path;
	argv[2] = '\0';

	GError* err = NULL;
	gdk_spawn_on_screen(screen, pwd, argv, NULL,
						G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_STDOUT_TO_DEV_NULL |
						G_SPAWN_STDERR_TO_DEV_NULL | G_SPAWN_SEARCH_PATH,
						NULL, NULL, NULL, &err);


	g_free (path);
	g_free (pwd);
}

static void
edit_with_vim_callback (NautilusMenuItem *item,
			NautilusFileInfo *file_info)
{
	edit_with_vim (item, file_info);
}

static NautilusMenuItem *
edit_with_vim_menu_item_new (NautilusFileInfo *file_info,
			     GdkScreen        *screen)
{
	NautilusMenuItem *ret;
	char *action_name = _("NautilusEditWithVim::open_vim");
	const char *name = _("Edit with Vim");
	const char *tooltip = _("Open this file in Vim");


	ret = nautilus_menu_item_new (action_name, name, tooltip, NULL);

	g_object_set_data (G_OBJECT (ret),
			   "NautilusEditWithVim::screen",
			   screen);

	g_signal_connect (ret, "activate",
			  G_CALLBACK (edit_with_vim_callback),
			  file_info);

	return ret;
}

static GList *
nautilus_edit_with_vim_get_background_items (NautilusMenuProvider *provider,
					     GtkWidget		  *window,
					     NautilusFileInfo	  *file_info)
{
	/* we don't do anything with the menu when the user clicks the background */
	return NULL;
}

GList *
nautilus_edit_with_vim_get_file_items (NautilusMenuProvider *provider,
				       GtkWidget            *window,
				       GList                *files)
{
	GList *items;

	if (g_list_length (files) != 1 ||
	    nautilus_file_info_is_directory (files->data) ||
	    nautilus_file_info_get_file_type (files->data) == G_FILE_TYPE_SHORTCUT ||
	    nautilus_file_info_get_file_type (files->data) == G_FILE_TYPE_MOUNTABLE) {
		return NULL;
	}

	items = NULL;

	GFile* file = nautilus_file_info_get_location (files->data);
	char* path = g_file_get_path(file);

	items = g_list_append(items,
		      edit_with_vim_menu_item_new (
				  files->data,
				  gtk_widget_get_screen (window)
				  )
			);

	g_free (path);

	return items;
}

static void
nautilus_edit_with_vim_menu_provider_iface_init (NautilusMenuProviderIface *iface)
{
	iface->get_background_items = nautilus_edit_with_vim_get_background_items;
	iface->get_file_items = nautilus_edit_with_vim_get_file_items;
}

static void 
nautilus_edit_with_vim_instance_init (NautilusEditWithVim *cvs)
{
}

static void
nautilus_edit_with_vim_class_init (NautilusEditWithVimClass *class)
{
}

static void
nautilus_edit_with_vim_class_finalize (NautilusEditWithVimClass *class)
{
}

GType
nautilus_edit_with_vim_get_type (void) 
{
	return editwithvim_type;
}

void
nautilus_edit_with_vim_register_type (GTypeModule *module)
{
	static const GTypeInfo info = {
		sizeof (NautilusEditWithVimClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) nautilus_edit_with_vim_class_init,
		(GClassFinalizeFunc) nautilus_edit_with_vim_class_finalize,
		NULL,
		sizeof (NautilusEditWithVim),
		0,
		(GInstanceInitFunc) nautilus_edit_with_vim_instance_init,
	};

	static const GInterfaceInfo menu_provider_iface_info = {
		(GInterfaceInitFunc) nautilus_edit_with_vim_menu_provider_iface_init,
		NULL,
		NULL
	};

	editwithvim_type = g_type_module_register_type (module,
						     G_TYPE_OBJECT,
						     "NautilusEditWithVim",
						     &info, 0);

	g_type_module_add_interface (module,
				     editwithvim_type,
				     NAUTILUS_TYPE_MENU_PROVIDER,
				     &menu_provider_iface_info);
}
