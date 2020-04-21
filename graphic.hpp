#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED
#include <math.h>

GtkWidget* create_one_dimensional_graphic(int *massiv, int size, int width, int height);

class graphic_parameters
{
    cairo_t *cr; ///
    cairo_path_t *axes; ///оси и надписи
    cairo_path_t *shtrihi;
    cairo_path_t *body;
    char *y_text="Ось y";
    char *x_text="Ось х";
    int text_size=13;
    int width=400; ///общий размер виджета
    int height=290;
    float w; ///рабочая область
    float h;
    float x_null;
    float y_null;
    int gap=6;
    float arrow_len=15;           ///размер стрелок
    float arrow_angle=M_PI/14;
};

#endif // GRAPHIC_H_INCLUDED
