/*
 *  nautilus-edit-with-vim.h
 * 
 *  Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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
 *  Author: Christian Neumair <chris@gnome-de.org>
 * 
 */

#ifndef NAUTILUS_EDIT_WITH_VIM_H
#define NAUTILUS_EDIT_WITH_VIM_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_EDIT_WITH_VIM	  (nautilus_edit_with_vim_get_type ())
#define NAUTILUS_OPEN_TERMINAL(o)	  (G_TYPE_CHECK_INSTANCE_CAST ((o), NAUTILUS_TYPE_EDIT_WITH_VIM, NautilusEditWithVim))
#define NAUTILUS_IS_EDIT_WITH_VIM(o)	  (G_TYPE_CHECK_INSTANCE_TYPE ((o), NAUTILUS_TYPE_EDIT_WITH_VIM))
typedef struct _NautilusEditWithVim      NautilusEditWithVim;
typedef struct _NautilusEditWithVimClass NautilusEditWithVimClass;

struct _NautilusEditWithVim {
	GObject parent_slot;
};

struct _NautilusEditWithVimClass {
	GObjectClass parent_slot;
};

GType nautilus_edit_with_vim_get_type      (void);
void  nautilus_edit_with_vim_register_type (GTypeModule *module);

G_END_DECLS

#endif
