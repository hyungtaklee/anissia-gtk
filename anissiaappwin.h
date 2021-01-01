#ifndef __ANISSIAPPWIN_H
#define __ANISSIAAPWIN_H

#include <gtk/gtk.h>
#include "anissiaapp.h"

#define ANISSIA_APP_WINDOW_TYPE (anissia_app_window_get_type())
G_DECLARE_FINAL_TYPE(AnissiaAppWindow, anissia_app_window, ANISSIA, APP_WINDOW, GtkApplicationWindow);

AnissiaAppWindow *anissia_app_window_new(AnissiaApp *app);
void anissia_app_window_open(AnissiaAppWindow *win, GFile *file);

#endif /* __ANISSIAAPPWIN_H */