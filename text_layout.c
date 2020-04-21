#include <gtk/gtk.h>
#include "text_layout.h"

PangoLayout* get_width_height_of_text(char *text, char *font, float size, float *w, float *h)
{
    GdkScreen *screen = gdk_screen_get_default();
    PangoContext *context = gdk_pango_context_get_for_screen (screen);
    PangoLayout *layout = pango_layout_new (context);
    if(g_utf8_validate(text,-1,0))
    {
        pango_layout_set_text(layout,text,-1);
        PangoFontDescription *desc=pango_font_description_new();
        pango_font_description_set_family(desc,font);
        pango_font_description_set_size(desc,size*1024);
        pango_layout_set_font_description (layout, desc);
        int width=0,height=0;
        pango_layout_get_size(layout, &width, &height);
        *w=(float) width/1024;
        *h=(float) height/1024;
        pango_font_description_free(desc);
    }
    else
    {
        printf("Текст не является валидным в кодировке UTF8\n");
    }
    return layout;
}

text_layout::text_layout(char *text_, char *font_, int fontsize_)
{
    text=g_string_new(text_);
    font=g_string_new(font_);
    fontsize=fontsize_;
    float w=0,h=0;
    layout=get_width_height_of_text(text_,font_,(float) fontsize_,&w, &h);
    width=(int) w;
    height=(int) h;
}

text_layout::text_layout(float num, char *font_, int fontsize_)
{
    text=g_string_new(NULL);
    g_string_printf(text, "%.2f", num);
    font=g_string_new(font_);
    fontsize=fontsize_;
    float w=0,h=0;
    layout=get_width_height_of_text(text->str,font_,(float) fontsize_,&w, &h);
    width=(int) w;
    height=(int) h;
}

text_layout::~text_layout()
{
    g_string_free(text,TRUE);
    g_string_free(font, TRUE);
    g_object_unref(layout);
}

void text_layout::change_text_font_fontsize(char *new_text, char *new_font, int new_fontsize)
{
    float w=0, h=0;
    if(new_text!=0||new_font!=0||new_fontsize!=0) g_object_unref(layout);
    if(new_text!=0&&new_font!=0&&new_fontsize!=0)
    {
        layout=get_width_height_of_text(new_text, new_font, new_fontsize, &w,&h);
        g_string_erase(text,0,-1);
        g_string_append(text, new_text);
        g_string_erase(font,0,-1);
        g_string_append(font, new_font);
        fontsize=new_fontsize;
    } else if(new_text!=0&&new_font!=0&&new_fontsize==0)
    {
        layout=get_width_height_of_text(new_text, new_font, fontsize, &w,&h);
        g_string_erase(text,0,-1);
        g_string_append(text, new_text);
        g_string_erase(font,0,-1);
        g_string_append(font, new_font);
    } else if(new_text!=0&&new_font==0&&new_fontsize!=0)
    {
        layout=get_width_height_of_text(new_text, font->str, new_fontsize, &w,&h);
        g_string_erase(text,0,-1);
        g_string_append(text, new_text);
        fontsize=new_fontsize;
    } else if(new_text==0&&new_font!=0&&new_fontsize!=0)
    {
        layout=get_width_height_of_text(text->str, new_font, new_fontsize, &w,&h);
        g_string_erase(font,0,-1);
        g_string_append(font, new_font);
        fontsize=new_fontsize;
    } else if(new_text==0&&new_font==0&&new_fontsize!=0)
    {
        layout=get_width_height_of_text(text->str, font->str, new_fontsize, &w,&h);
        fontsize=new_fontsize;
    } else if(new_text==0&&new_font!=0&&new_fontsize==0)
    {
        layout=get_width_height_of_text(text->str, new_font, fontsize, &w,&h);
        g_string_erase(font,0,-1);
        g_string_append(font, new_font);
    } else if(new_text!=0&&new_font==0&&new_fontsize==0)
    {
        layout=get_width_height_of_text(new_text, font->str, fontsize, &w,&h);
        g_string_erase(text,0,-1);
        g_string_append(text, new_text);
    }
    width=(int) w;
    height=(int) h;
}
