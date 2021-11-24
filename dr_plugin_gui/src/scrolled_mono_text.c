#include "scrolled_mono_text.h"
#include <gtk/gtk.h>
#include "mono_text.h"

GtkScrolledWindow* add_scrolled_mono_text(GtkDialog* dialog, char* text, unsigned text_length)
{
    GtkScrolledWindow* scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
    GtkTextView* text_view = create_mono_text(text, text_length);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(text_view));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(dialog)), GTK_WIDGET(scrolled_window), TRUE, TRUE, 0);
    return scrolled_window;
}
