#ifndef TEXT_LAYOUT_H_INCLUDED
#define TEXT_LAYOUT_H_INCLUDED

#include <gtk/gtk.h>

PangoLayout* get_width_height_of_text(char *text, char *font, float size, float *w, float *h);

class text_layout
{
    private:
    int fontsize;
    public:
    GString *text;
    GString *font;
    PangoLayout *layout;
    int width;
    int height;
    text_layout(char *text, char *font, int fontsize);
    void change_text_font_fontsize(char *new_text, char *new_font, int new_fontsize);
    ~text_layout();
    text_layout(float num, char *font, int fontsize);
};


#endif // TEXT_LAYOUT_H_INCLUDED
