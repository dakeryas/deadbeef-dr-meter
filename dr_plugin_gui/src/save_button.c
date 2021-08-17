#include <gtk/gtk.h>
#include <glib.h>
#include "save_button.h"
#include "dr_run_data.h"

static void write_log(const char* log_text, const char* filename)
{
    FILE* file = fopen(filename, "w");
    fprintf(file, "%s", log_text);
    fclose(file);
}

#if GTK_CHECK_VERSION(3,0,0)
#define GTK_DOMAIN "gtk30"
#else
#define GTK_DOMAIN "gtk20"
#endif
static GtkFileChooser* create_save_dialog(GtkWindow* log_dialog)
{
    GtkFileChooser* dialog = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new("Save", log_dialog, GTK_FILE_CHOOSER_ACTION_SAVE, g_dgettext(GTK_DOMAIN, "_Cancel"), GTK_RESPONSE_CANCEL, g_dgettext(GTK_DOMAIN, "_Save"), GTK_RESPONSE_ACCEPT, NULL));
    gtk_file_chooser_set_do_overwrite_confirmation(dialog, TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), log_dialog);
    return dialog;
}
#undef GTK_DOMAIN

static void open_save_dialog(GtkButton*, gpointer data)
{
    dr_run_data_t* run_data = (dr_run_data_t*)data;
    GtkFileChooser* file_dialog = create_save_dialog(run_data->dr_dialog);
    if(gtk_dialog_run(GTK_DIALOG(file_dialog)) == GTK_RESPONSE_ACCEPT)
    {
        gchar* filename;
        filename = gtk_file_chooser_get_filename(file_dialog);
        write_log(run_data->log, filename);
        g_free(filename);
    }
    gtk_widget_destroy(GTK_WIDGET(file_dialog));
}

GtkWidget* create_save_button(dr_run_data_t* run_data)
{
    GtkWidget* save_button = gtk_button_new_with_label("Save DR log");
    g_signal_connect(save_button, "clicked", G_CALLBACK(open_save_dialog), run_data);
    return save_button;
}
