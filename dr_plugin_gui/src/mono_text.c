#include "mono_text.h"
#include <gtk/gtk.h>

static void set_monospace(GtkTextView* text)
{
#if GTK_CHECK_VERSION(3,0,0)
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(text));
    GtkCssProvider *provider = gtk_css_provider_new();
    char css[] = "textview {font-family: \"monospace\";}";
    gtk_css_provider_load_from_data(provider, css, sizeof(css), NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#else
    PangoFontDescription *descr = pango_font_description_from_string("monospace");
    gtk_widget_modify_font(GTK_WIDGET(text), descr);
    pango_font_description_free(descr);
#endif
}

GtkTextView* create_mono_text(char* log_buffer, unsigned log_length)
{
    GtkTextView* text = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(text), log_buffer, log_length);
    set_monospace(text);
    gtk_text_view_set_editable(text, FALSE);
    return text;
}
