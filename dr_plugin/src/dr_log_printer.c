#include "dr_log_printer.h"
#include "thread_data.h"

unsigned sprint_item_dr_log_printer(dr_log_printer_t* self, thread_datum_t* datum, char* begin, char endline)
{
    char* end = begin;
    end += sprint_dr_stats(&datum->dr_stats, end);
    end += sprintf(end, "      ");
    end += self->sprint_track_info(datum->item, end);
    end += sprintf(end, "%c", endline);
    return end - begin;
}

unsigned sprintl_item_dr_log_printer(dr_log_printer_t* self, thread_datum_t* datum, char* begin)
{
    return sprint_item_dr_log_printer(self, datum, begin, '\n');
}

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

unsigned sprint_col_headers_dr_log_printer(dr_log_printer_t* self, char* begin, char endline)
{
    (void) self;
    return sprintf(begin, "DR     Peak        RMS         Duration  Track%c", endline);
}

unsigned sprintl_col_headers_dr_log_printer(dr_log_printer_t* self, char* begin)
{
    return sprint_col_headers_dr_log_printer(self, begin, '\n');
}

unsigned sprint_log_dr_log_printer(dr_log_printer_t* self, thread_data_t* thread_data, char* begin)
{
    char* end = begin;
    unsigned line_length = 105;
    end += sprintl_line(line_length, end);
    end += sprintl_col_headers_dr_log_printer(self, end);
    end += sprintl_line(line_length, end);
    for(unsigned k = 0; k < thread_data->items; ++k)
        end += sprintl_item_dr_log_printer(self, thread_data->data + k, end);
    end += sprintl_line(line_length, end);
    return end - begin;
}
