#include <gtk/gtk.h>
#include "graphic.h"
#include "text_layout.h"

graphic_parameters::~graphic_parameters()
{
    if(massiv) g_free(massiv);
    gtk_widget_destroy(eventbox);
    y_text.~text_layout();
    x_text.~text_layout();
    number->~text_layout();
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}


 void adj_changed_cb (GtkAdjustment *adjustment, gpointer user_data)
 {
    graphic_parameters *param=(graphic_parameters*) user_data;
    double value=gtk_adjustment_get_value(adjustment);
    printf("adj_value=%f\n", value);
    int elements=param->len-param->count_in_display; ///количество значений массива, которое не влезает на экран
    elements=abs(elements);
        float k=100.0/elements;
        param->offset=round(value/k);
        printf("k=%f|elemetns=%d|len=%d|count_dispaly=%d|offset=%d\n",k, elements,param->len,param->count_in_display, param->offset);
        param->change_graphic_adj();
    if(param->offset>param->len) param->offset=param->len;
 }

 gboolean eventbox_scroll_cb (GtkWidget *widget, GdkEvent *event, gpointer  user_data)
 {
    graphic_parameters *gr=(graphic_parameters*) user_data;
    double direction=0;
    gdk_event_get_scroll_deltas(event, NULL, &direction);
    int direction_=(int) direction;
    if(direction_==-1)
    {
        gr->multiplier_x=gr->multiplier_x+gr->multiplier_x*0.02;
    }
    if(direction_==1)
    {
        gr->multiplier_x=gr->multiplier_x-gr->multiplier_x*0.02;
    }
    printf("multiplier=%f\n", gr->multiplier_x);
    gr->change_graphic_adj();
    return FALSE;
 }

 void graphic_parameters::create_vertical_line()
 {
    if(pos>=offset&&pos<len)
    {
        //cairo_set_line_width(cr,0.8);
        cairo_set_source_rgb(cr, 0, 0, 1);
        cairo_move_to(cr, x_null+(pos-offset)*multiplier_x, y_null);
        cairo_line_to(cr, x_null+(pos-offset)*multiplier_x, margin);
        cairo_set_line_width(cr,2);
        cairo_stroke(cr);
        cairo_set_source_rgb(cr,0,0,0);
        if(!number)
        number = new text_layout(massiv[pos],"Liberation Serif", 12);
        else
        {
            number->text=g_string_erase(number->text,0,-1);
            g_string_printf(number->text, "%.2f", massiv[pos]);
            number->change_text_font_fontsize(number->text->str,0,0);
        }
        cairo_move_to(cr, x_null+(pos-offset)*multiplier_x, 0);
        pango_cairo_show_layout(cr, number->layout);
    }
 }

 gboolean eventbox_press_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
 {

    graphic_parameters *gr=(graphic_parameters*) user_data;
    gr->pos=round((event->button.x-gr->x_null)/gr->multiplier_x)+gr->offset;
    if(gr->pos<0) gr->pos=0;
    gr->change_graphic_adj();
    return FALSE;
 }

graphic_parameters::graphic_parameters(int width, int height)
{
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surface);

    widget_width=width;
    widget_height=height;
    create_axes_and_xy_labels();
    cairo_stroke(cr);

    eventbox=gtk_event_box_new();
    g_signal_connect(eventbox,"button-press-event", G_CALLBACK(eventbox_press_cb), this);
    GtkAdjustment *adj_h=gtk_adjustment_new(0,0,100,1,5,10);
    GtkAdjustment *adj_v=gtk_adjustment_new(0,0,100,1,5,10);
    GtkWidget *viewport=gtk_viewport_new(adj_h, adj_v);
    scrolledwindow=gtk_scrolled_window_new(adj_h, adj_v);
    g_object_set(scrolledwindow, "hscrollbar-policy", GTK_POLICY_EXTERNAL, "vscrollbar-policy", GTK_POLICY_EXTERNAL, NULL);
    gtk_container_add(GTK_CONTAINER(viewport), scrolledwindow);

    gtk_widget_set_events(scrolledwindow, GDK_SCROLL_MASK);  ///для примера, по умолчанию включено
    g_signal_connect(scrolledwindow,"scroll-event",G_CALLBACK(eventbox_scroll_cb), this);

    GtkWidget *box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

    adj=gtk_adjustment_new(0,0,110,1,5,10);
    g_signal_connect(adj,"value-changed", G_CALLBACK(adj_changed_cb), this);
    scrollbar=gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL,adj);
    gtk_box_pack_end(GTK_BOX(box),scrollbar, FALSE,FALSE,0);

    image_from_surface=gtk_image_new_from_surface(surface);
    gtk_container_add(GTK_CONTAINER(scrolledwindow),image_from_surface);
    gtk_box_pack_start(GTK_BOX(box),viewport, TRUE,TRUE,0);
    gtk_container_add(GTK_CONTAINER(eventbox),box);
}

void graphic_parameters::resize_graphic(int new_width, int new_height)
{
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, new_width, new_height);
    cr = cairo_create(surface);
    widget_width=new_width;
    widget_height=new_height;
    create_axes_and_xy_labels();
    //cairo_set_line_width(cr,0.8);
    //cairo_set_source_rgb(cr, 0, 0, 1);
    update_graphic(offset);
    cairo_stroke(cr);
    create_vertical_line();
    gtk_image_set_from_surface(GTK_IMAGE(image_from_surface),surface);
}

void graphic_parameters::change_graphic_adj(void)
{
    cairo_set_source_rgb(cr,0.8,0.8,0.8);
    cairo_paint(cr);
    create_axes_and_xy_labels();
    update_graphic(offset);
    cairo_stroke(cr);
    create_vertical_line();
    gtk_image_set_from_surface(GTK_IMAGE(image_from_surface),surface);
}


void graphic_parameters::create_axes_and_xy_labels(void)
{
    cairo_set_source_rgb(cr, 0, 0, 0); ///чёрный
    cairo_select_font_face(cr, y_text.font->str, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    ///вывод надписи оси Y
    cairo_move_to(cr, y_text.height+2, widget_height*0.5-y_text.width*0.5);
    cairo_rotate(cr,3.14/2);
    pango_cairo_show_layout(cr,y_text.layout);
    cairo_rotate(cr,-3.14/2); ///обратный поворот

    ///вывод надписи оси X
    cairo_select_font_face(cr, y_text.font->str, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_move_to(cr, widget_width*0.5-x_text.width*0.5, widget_height-y_text.height);
    pango_cairo_show_layout(cr,x_text.layout);

     ///Оси
     float arrow_len=15;           ///размер стрелок
    float arrow_angle=M_PI/14;
    float char_h=x_text.height;
    cairo_move_to(cr,char_h+gap_x,margin);
    cairo_line_to(cr,char_h+gap_x,widget_height-char_h-gap_y);
    cairo_move_to(cr,widget_width-margin,widget_height-char_h-gap_y);
    cairo_line_to(cr,char_h+gap_x,widget_height-char_h-gap_y);

    ///стрелочка на оси x
    cairo_move_to(cr,widget_width-margin,widget_height-char_h-gap_y);
    cairo_line_to(cr,widget_width-arrow_len-margin,widget_height-char_h-gap_y+arrow_len*tan(arrow_angle));
    cairo_move_to(cr,widget_width-margin,widget_height-char_h-gap_y);
    cairo_line_to(cr,widget_width-arrow_len-margin,widget_height-char_h-gap_y-arrow_len*tan(arrow_angle));

    ///стрелочка на оси y
    cairo_move_to(cr,char_h+gap_x,margin);
    cairo_line_to(cr,char_h+gap_x+arrow_len*tan(arrow_angle),margin+arrow_len);
    cairo_move_to(cr,char_h+gap_x,margin);
    cairo_line_to(cr,char_h+gap_x-arrow_len*tan(arrow_angle),margin+arrow_len);
}

void graphic_parameters::create_one_dimensional_graphic(float *array, int len_)
{
    massiv=(float*) g_malloc0(len_);
    memcpy(massiv,array,len_);
    len=round(len_/sizeof(float));
}

void graphic_parameters::update_graphic(int offset)
{
    if(massiv!=0)
{
     ///начальная точка графика
    float char_h=x_text.height;
    float width_=widget_width-margin-char_h-gap_x; ///ширина с учётом смещения
    float height_=widget_height-char_h-gap_y-margin;

    x_null=char_h+gap_x;
    y_null=widget_height-char_h-gap_y;
    printf("x_null=%f, y_null=%f\n", x_null, y_null);

    count_in_display=ceil(width_/multiplier_x);
    int i=offset;

    // float max=0;
    // float min=0;
    int end_of_count=MIN(count_in_display, len);
        if(end_of_count-1+offset<len)
        {
            for(i; i<end_of_count-1+offset; ++i)
            {
                max=MAX(massiv[i],max);
            }
            min=max;
            i=offset;
            for(i; i<end_of_count-1+offset; ++i)
            {
                min=MIN(massiv[i],min);
            }
        }
    if(max_&&min)
    {
        max_->text=g_string_erase(max_->text,0,-1);
        g_string_printf(max_->text, "%.2f", max);
        min_->text=g_string_erase(min_->text,0,-1);
        g_string_printf(min_->text, "%.2f", min);
        max_->change_text_font_fontsize(max_->text->str,0,0);
        min_->change_text_font_fontsize(min_->text->str,0,0);

    } else
    {
        max_=new text_layout(max,"Liberation Serif", 12);
        min_=new text_layout(min,"Liberation Serif", 12);
    }
    cairo_move_to(cr, 0, 0);
    pango_cairo_show_layout(cr, max_->layout);
    cairo_move_to(cr, 0, y_null);
    pango_cairo_show_layout(cr, min_->layout);

    printf("min=%f\n", min);
    printf("max=%f\n", max);
    ///количество пикселей на единицу измерения
    float multiplier_y=height_/(max-min);
    i=0;
    for(i;i<end_of_count-1;i++)
    {
        int j=offset; ///смещение от начала буфера
        if(j+i<end_of_count-1+offset)
        {
            float m=(i)*multiplier_x;
            float m_1=(i+1)*multiplier_x;
            if(i+j+1<len)
            {
                cairo_move_to(cr,x_null+m,(y_null-(massiv[i+j]-min)*multiplier_y));
                cairo_line_to(cr,x_null+m_1,(y_null-(massiv[i+j+1]-min)*multiplier_y));
            }
        }
    }
}
}
