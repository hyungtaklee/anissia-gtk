#include <gtk/gtk.h>

#include "anissiaapp.h"
#include "anissiaappwin.h"
#include "anissiaapplist.h"

struct _AnissiaAppWindow
{
    GtkApplicationWindow parent;
};

typedef struct _AnissiaAppWindowPrivate AnissiaAppWindowPrivate;

struct _AnissiaAppWindowPrivate
{
    GtkWidget *stack;
};

G_DEFINE_TYPE_WITH_PRIVATE(AnissiaAppWindow, anissia_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void anissia_app_window_init(AnissiaAppWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
}

static void anissia_app_window_class_init(AnissiaAppWindowClass *class)
{
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                                "/com/taskidle/anissiaapp/window.ui");
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), AnissiaAppWindow, stack);
}

AnissiaAppWindow *anissia_app_window_new(AnissiaApp *app)
{
    return g_object_new(ANISSIA_APP_WINDOW_TYPE, "application", app, NULL);
}

void anissia_app_window_open(AnissiaAppWindow *win, cJSON *json, const gchar *basename)
{
    AnissiaAppWindowPrivate *priv;
    GtkTreeModel *model = NULL;
    GtkWidget *scrolled, *treeview;

    /* get private window of main window */
    priv = anissia_app_window_get_instance_private(win);

    /* create scroll view */
    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolled);
    gtk_widget_set_hexpand(scrolled, TRUE);
    gtk_widget_set_vexpand(scrolled, TRUE);

    /* create tree model */
    model = anissia_app_list_create_model(json);

    /* crate_tree_view */
    treeview = gtk_tree_view_new_with_model(model);
    gtk_tree_view_set_search_column(GTK_TREE_VIEW(treeview), COLUMN_TITLE);

    g_object_unref(model);

    gtk_container_add(GTK_CONTAINER(scrolled), treeview);

    /* add colums to the tree view */
    anissia_app_list_add_columns(GTK_TREE_VIEW(treeview));
    gtk_widget_show(treeview);

    /* add list view to scroll view */
    gtk_stack_add_titled(GTK_STACK(priv->stack), scrolled, basename, basename);
}