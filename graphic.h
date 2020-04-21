#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <math.h>
#include "text_layout.h"

class graphic_parameters
{
    private:
     text_layout y_text=text_layout("Ось y","Liberation Serif", 14);
     text_layout x_text=text_layout("Ось x","Liberation Serif", 14);
     text_layout *number=0; ///текущее число для отображения
     float max=0;
     float min=0;
     text_layout *max_=0;
     text_layout *min_=0;

    GtkAdjustment *adj;
    GtkWidget *scrollbar;
     float gap_x=25; ///зазор между надписью и осями
     float gap_y=5; ///зазор между надписью и осями

    void create_axes_and_xy_labels(void);

    public:
    cairo_t *cr;
    float *massiv=0; ///массив для графика
    int len=0; ///длина массива
    int count_in_display=0; ///отображаемое количество элементов массива
    float multiplier_x=6;
    int offset=0;
    float x_null=0;
    float y_null=0;
    int pos=0;/// вертикальная линия
    float margin=16;

    int callback_width;  ///новые размеры
    int callback_height;
    int widget_width;
    int widget_height;
    int scroll_height=0;
    GtkWidget *eventbox;
    GtkWidget *scrolledwindow;
    GtkWidget *image_from_surface;
    cairo_surface_t *surface;

    graphic_parameters(int width, int height);
    ~graphic_parameters();
    void resize_graphic(int new_width, int new_height);
    void create_one_dimensional_graphic(float *massiv, int size);
    void update_graphic(int offset);
    void change_graphic_adj(void);
    void create_vertical_line(void);
};



#endif // GRAPHIC_H_INCLUDED
