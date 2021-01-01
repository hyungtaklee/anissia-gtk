#include <gtk/gtk.h>

#include "anissiaapp.h"
#include "anissiaappwin.h"

struct _AnissiaAppWindow
{
    GtkApplicationWindow parent;
};

G_DEFINE_TYPE(AnissiaAppWindow, anissia_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void anissia_app_window_init(AnissiaAppWindow *app)
{
}

static void anissia_app_window_class_init(AnissiaAppWindowClass *class)
{
}

AnissiaAppWindow *anissia_app_window_new(AnissiaApp *app)
{
    return g_object_new(ANISSIA_APP_WINDOW_TYPE, "application", app, NULL);
}

void anissia_app_window_open(AnissiaAppWindow *win, GFile *file)
{
}