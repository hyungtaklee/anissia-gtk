#include <gtk/gtk.h>

#include "anissiaapp.h"
#include "anissiaappwin.h"
#include "requester.h"

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
    GList *windows;
    AnissiaAppWindow *win;
    int i;
    struct request_data *json_data[7];
    gchar *day_name[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

    win = anissia_app_window_new(ANISSIA_APP(app));
    // gtk_window_present(GTK_WINDOW(win));

    windows = gtk_application_get_windows(GTK_APPLICATION(app));
    if (windows)
        win = ANISSIA_APP_WINDOW(windows->data);
    else
        win = anissia_app_window_new(ANISSIA_APP(app));
    
    /* run anissia-cli and save to each file */
    for (i = 0; i < 7; i++) {
        /* create data structure for later run */
        json_data[i] = requester_create_data_struct();
        
        requester_init(json_data[i]);
        requester_request(json_data[i], i, -1, -1);
        parse_to_json(json_data[i]);
    
        /* make stack window for each days */
        anissia_app_window_open(win, json_data[i]->json_data, day_name[i]);
    }

    /* deallocate resources */
    for (i = 0; i < 7; i++) {
        if (json_data[i]->json_data != NULL)
            cJSON_Delete(json_data[i]->json_data);
        free(json_data[i]);
    }

    gtk_window_present(GTK_WINDOW(win));

}

static void anissia_app_class_init(AnissiaAppClass *class)
{
    G_APPLICATION_CLASS(class)->activate = anissia_app_activate;
}

AnissiaApp *anissia_app_new(void)
{
    return g_object_new(ANISSIA_APP_TYPE, "application-id",
                        "com.taskidle.anissiaapp", "flags",
                        G_APPLICATION_HANDLES_OPEN, NULL);
}