#include <gtk/gtk.h>

#include "anissiaapp.h"
#include "anissiaappwin.h"

struct _AnissiaApp
{
    GtkApplication parent;
};

G_DEFINE_TYPE(AnissiaApp, anissia_app, GTK_TYPE_APPLICATION);

static void anissia_app_init(AnissiaApp *app)
{
}

static void anissia_app_activate(GApplication *app)
{
    AnissiaAppWindow *win;

    win = anissia_app_window_new(ANISSIA_APP(app));
    gtk_window_present(GTK_WINDOW(win));
}

static void anissia_app_open(GApplication *app, GFile **files,
                             gint n_files, const gchar *hint)
{
    GList *windows;
    AnissiaAppWindow *win;
    int i;

    windows = gtk_application_get_window(GTK_APPLICATION(app));
    if (windows)
        win = ANISSIA_APP_WINDOW(windows->data);
    else
        win = anissia_app_window_new(ANISSIA_APP(app));
    

}

static void anissia_app_class_init(AnissiaAppClass *class)
{
    G_APPLICATION_CLASS(class)->activate = anissia_app_activate;
    G_APPLICATION_CLASS(class)->open = anissia_app_open;
}

AnissiaApp *anissia_app_new(void)
{
    return g_object_new(ANISSIA_APP_TYPE, "application-id",
                        "com.taskidle.anissiaapp", "flags",
                        G_APPLICATION_HANDLES_OPEN, NULL);
}