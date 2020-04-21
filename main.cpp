#include "main.hpp"

externals::externals()
{
    ;;
}

gboolean configure_event_cb (GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    int height=event->configure.height;
    int width=event->configure.width;
    appdata *data=(appdata*) user_data;
    GtkAllocation alloc;
    GtkWidget *first=gtk_notebook_get_nth_page(data->notebook,0);
    gtk_widget_get_allocation(data->param->scrolledwindow,&alloc);

    if(data->param->scroll_height==0)
    data->param->scroll_height=alloc.height-data->param->widget_height;

    if(data->param->callback_width!=alloc.width||data->param->callback_height!=alloc.height)
    {data->param->resize_graphic(alloc.width,alloc.height);}

    data->param->callback_width=alloc.width;
    data->param->callback_height=alloc.height;

    return FALSE;
}

appdata::appdata()
{
    GError *error = NULL;
    GtkBuilder *builder = gtk_builder_new();

    /*if (!gtk_builder_add_from_resource (builder, "/com/example/YourApp/window.glade", &error))
    {
        // загрузить файл не удалось
        g_critical ("Не могу загрузить файл: %s", error->message);
        g_error_free (error);
    }*/
    if (!gtk_builder_add_from_file (builder, "window.glade", &error))
    {
        // загрузить файл не удалось
        g_critical ("Не могу загрузить файл: %s", error->message);
        g_error_free (error);
    }
    win=GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    g_signal_connect(win,"configure-event",G_CALLBACK(configure_event_cb),this);
    gtk_widget_set_events(win,GDK_LEAVE_NOTIFY_MASK|
        GDK_ENTER_NOTIFY_MASK|GDK_STRUCTURE_MASK|GDK_POINTER_MOTION_MASK);

    notebook=GTK_NOTEBOOK(gtk_builder_get_object(builder, "notebook1"));

    menubar=GTK_MENU_BAR(gtk_builder_get_object(builder, "menubar1"));
    GtkWidget *button=GTK_WIDGET(gtk_builder_get_object(builder, "button1")); ///заполняющий виджет
    gtk_widget_destroy(button);
    gtk_builder_connect_signals (builder,this);
    g_clear_object(&builder);
}

void application_activate(GtkApplication *application, gpointer user_data)
{
    appdata *data=(appdata*) user_data;
    gtk_widget_set_size_request(data->win,150,150);
    gtk_application_add_window(data->app,GTK_WINDOW(data->win));

   graphic_parameters* param=new graphic_parameters(250,250);
    data->param=param; ///сохранение ссылки в класс

    float buf[]={-33, 4,55,11,114,100,220,311,5,8,44,71,66,300,290,200,188, 48, 11,40,0,55,11,114,100,220,311,5,8,44,71,66,300,290,200,188, 48, 11,40,0
    ,55,11,114,100,220,311,5,8,44,71,66,300,290,200,188, 48, 11,40,0,55,11,114,100,220,311,5,8,44,71,66,300,290,200,188, 48, 11,40,-88.88, 50, 44, 48};
    param->create_one_dimensional_graphic(buf,sizeof(buf));
    gtk_notebook_append_page(data->notebook,data->param->eventbox,0);

    /*    GValue val = G_VALUE_INIT;
    g_value_init(&val, G_TYPE_BOOLEAN);
    g_value_set_boolean(&val, TRUE);
    gtk_container_child_set_property(GTK_CONTAINER(data->notebook), gr, "tab-expand", &val);*/

    gtk_widget_show_all(data->win);
}

void application_shutdown(const GtkApplication *restrict application, gpointer user_data)
{

}

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);
  ///  int переменная;
    appdata data;
    gint res;
    data.app = gtk_application_new("gtk3.org", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(data.app, "activate", G_CALLBACK(application_activate), &data);
    g_signal_connect(data.app, "shutdown", G_CALLBACK(application_shutdown), &data);
    res = g_application_run(G_APPLICATION(data.app), 0, NULL);
    return 0;
}
