#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <gtk/gtk.h>
#include <string.h>
#include "graphic.h"

#define restrict __restrict__
#include <math.h>

class externals
{
    public:
    graphic_parameters *param;
    externals();
};

class appdata : public externals
{
    public:
    char *glade_name=(char*)"window.glade";
    GtkApplication *app;
    GtkWidget *win;
    GtkNotebook *notebook;
    GtkMenuBar *menubar;
    appdata();
};

#endif // MAIN_H_INCLUDED
