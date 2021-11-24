#include "mono_text.h"
#include <gtk/gtk.h>

static void set_monospace(GtkTextView* text_view)
{
#if GTK_CHECK_VERSION(3,0,0)
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(text_view));
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "textview {font-family: \"monospace\";}", -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#else
    PangoFontDescription *descr = pango_font_description_from_string("monospace");
    gtk_widget_modify_font(GTK_WIDGET(text_view), descr);
    pango_font_description_free(descr);
#endif
}

GtkTextView* create_mono_text(char* text, unsigned text_length)
{
    GtkTextView* text_view = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(text_view), text, text_length);
    set_monospace(text_view);
    gtk_text_view_set_editable(text_view, FALSE);
    return text_view;
}
