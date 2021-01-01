#ifndef __ANISSIAAPP_H
#define __ANISSIAAPP_H

#include <gtk/gtk.h>

#define ANISSIA_APP_TYPE (anissia_app_get_type())
G_DECLARE_FINAL_TYPE(AnissiaApp, anissia_app, ANISSIA, APP, GtkApplication)

AnissiaApp *anissia_app_new(void);

#endif /* __ANISSIAAPP_H */