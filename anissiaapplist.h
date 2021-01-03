#ifndef __ANISSIAAPPLIST_H
#define __ANISSIAAPPLIST_H

#include <gtk/gtk.h>

#include "cJSON.h"
#include "requester.h"

enum
{
    COLUMN_STATUS,
    COLUMN_TIME,
    COLUMN_TITLE,
    COLUMN_GENRE,
    COLUMN_START,
    COLUMN_END,
    COLUMN_URL,
    N_COLUMNS
};

extern GtkTreeModel *anissia_app_list_create_model(cJSON *json);
extern void anissia_app_list_add_columns(GtkTreeView *treeview);

#endif