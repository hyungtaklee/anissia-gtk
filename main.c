#include <gtk/gtk.h>

#include "anissiaapp.h"

int main(int argc, char *argv[])
{
    g_application_run(G_APPLICATION(anissia_app_new()), argc, argv);
}