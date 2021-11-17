#include "dr_log_printer.h"
#include "thread_data.h"

static unsigned sprint_n(char character, unsigned count, char* begin, char endline)
{
    char* end = begin;
    for(unsigned k = 0; k < count; ++k)
        end += sprintf(end, "%c", character);
    end += sprintf(end, "%c", endline);
    return end - begin;
}

static unsigned sprintl_n(char character, unsigned count, char* begin)
{
    return sprint_n(character, count, begin, '\n');
}

static unsigned sprintl_line(unsigned line_length, char* begin)
{
    return sprintl_n('-', line_length, begin);
}

static int sprint_dr_stats(const dr_log_printer_t* self, dr_stats_t* dr_stats, char* buffer)
{
    dr_stats_t stats = make_dB_dr_stats(dr_stats);
    return sprintf(buffer, self->dr_format, stats.dr, stats.peak, stats.rms);
}

unsigned sprint_item_dr_log_printer(const dr_log_printer_t* self, thread_datum_t* datum, char* begin, char endline)
{
    char* end = begin;
    end += sprint_dr_stats(self, &datum->dr_stats, end);
    end += sprintf(end, "     ");
    end += self->sprint_track_info(datum->item, end);
    end += sprintf(end, "%c", endline);
    return end - begin;
}

unsigned sprintl_item_dr_log_printer(const dr_log_printer_t* self, thread_datum_t* datum, char* begin)
{
    return sprint_item_dr_log_printer(self, datum, begin, '\n');
}

static unsigned sprint_album_info(const dr_log_printer_t* self, const thread_data_t* data, char* begin, char endline)
{
    char* end = begin;
    void* first_track = data->data[0].item;
    end += self->sprint_album_info(first_track, end);
    end += sprintf(end, "%c", endline);
    return end - begin;
}

static unsigned sprintl_album_info(const dr_log_printer_t* self, const thread_data_t* data, char* begin)
{
    return sprint_album_info(self, data, begin, '\n');
}

unsigned sprint_col_headers_dr_log_printer(const dr_log_printer_t* self, char* begin, char endline)
{
    (void) self;
    return sprintf(begin, "DR         Peak         RMS     Duration Track%c", endline);
}

unsigned sprintl_col_headers_dr_log_printer(const dr_log_printer_t* self, char* begin)
{
    return sprint_col_headers_dr_log_printer(self, begin, '\n');
}

unsigned sprint_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, char* begin, char endline)
{
    (void) self;
    char* end = begin;
    end += sprintf(end, "Number of tracks:  %i\n", items);
    end += sprintf(end, "Official DR value: DR%.0f%c", avg_dr, endline);
    return end - begin;
}

unsigned sprintl_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, char* begin)
{
    return sprint_summary_info(self, items, avg_dr, begin, '\n');
}

unsigned sprint_log_dr_log_printer(const dr_log_printer_t* self, const thread_data_t* thread_data, char* begin)
{
    char* end = begin;
    unsigned line_length = 80;
    end += sprintl_line(line_length, end);
    end += sprintl_album_info(self, thread_data, end);
    end += sprintl_line(line_length, end);
    end += sprintl_line(0, end);
    end += sprintl_col_headers_dr_log_printer(self, end);
    end += sprintl_line(line_length, end);
    double avg_dr = 0.;
    for(unsigned k = 0; k < thread_data->items; ++k)
    {
        thread_datum_t* datum = thread_data->data + k;
        avg_dr += datum->dr_stats.dr;
        end += sprintl_item_dr_log_printer(self, datum, end);
    }
    avg_dr /= thread_data->items;
    end += sprintl_line(line_length, end);
    end += sprintl_line(0, end);
    end += sprintl_summary_info(self, thread_data->items, avg_dr, end);
    end += sprintl_n('=', line_length, end);
    return end - begin;
}
