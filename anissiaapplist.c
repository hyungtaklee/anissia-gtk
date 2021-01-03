#include "anissiaapplist.h"

GtkTreeModel *anissia_app_list_create_model(cJSON *json)
{
    gint i = 0;
    GtkListStore *store;
    GtkTreeIter iter;
    cJSON *trav_ani = json->child;

    /* create list store */
    store = gtk_list_store_new(N_COLUMNS,
                               G_TYPE_STRING, /* for icon */
                               G_TYPE_STRING,
                               G_TYPE_STRING,
                               G_TYPE_STRING,
                               G_TYPE_STRING,
                               G_TYPE_STRING,
                               G_TYPE_STRING);

    /* add date to list store */
    while (trav_ani != NULL) {
        /* parsing json info date into struct */
        struct ani_info info;
        gchar *icon_name;
        if (!parse_json_info(&info, trav_ani))
            return NULL;

        /* setting icon for broadcasting status */
        if (info.status) { /* in progress */
            icon_name = "emblem-synchronizing";
        }
        else { /* stopped */
            icon_name = "dialog-warning";
        }
        
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                            COLUMN_STATUS, icon_name,
                            COLUMN_TIME, info.time,
                            COLUMN_TITLE, info.title,
                            COLUMN_GENRE, info.genre,
                            COLUMN_START, info.start_date,
                            COLUMN_END,  info.end_date,
                            COLUMN_URL, info.url,
                            -1);

        trav_ani = trav_ani->next;
    }
    return GTK_TREE_MODEL(store);
}

void anissia_app_list_add_columns(GtkTreeView *treeview)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);

    /* column for symbolic icon (status) */
    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new_with_attributes("Status",
                                                  renderer,
                                                  "icon-name",
                                                  COLUMN_STATUS,
                                                  NULL);
    /* set this column to a fixed sizing (of 50 pixels) */
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column),
                                    GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column), 55);
    gtk_tree_view_append_column(treeview, column);

    /* column for time */
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Time",
                                                      renderer,
                                                      "text",
                                                      COLUMN_TIME,
                                                      NULL);
    /* set this column to a fixed sizing (of 70 pixels) */
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column),
                                    GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column), 70);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_TIME);
    gtk_tree_view_append_column(treeview, column);

    /* column for title */
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Title",
                                                      renderer,
                                                      "text",
                                                      COLUMN_TITLE,
                                                      NULL);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_TITLE);
    gtk_tree_view_append_column(treeview, column);

}
