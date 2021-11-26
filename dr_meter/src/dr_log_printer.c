#include "dr_log_printer.h"
#include "tagged_dr_data.h"
#include "sample_mean.h"

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

unsigned sprint_item_dr_log_printer(const dr_log_printer_t* self, tagged_dr_datum_t* datum, char* begin, char endline)
{
    char* end = begin;
    end += sprint_dr_stats(self, &datum->dr_stats, end);
    end += sprintf(end, "     ");
    end += self->sprint_track_info(datum->item, end);
    end += sprintf(end, "%c", endline);
    return end - begin;
}

unsigned sprintl_item_dr_log_printer(const dr_log_printer_t* self, tagged_dr_datum_t* datum, char* begin)
{
    return sprint_item_dr_log_printer(self, datum, begin, '\n');
}

static unsigned sprint_album_info(const dr_log_printer_t* self, track_t* track, char* begin, char endline)
{
    char* end = begin;
    end += self->sprint_album_info(track, end);
    end += sprintf(end, "%c", endline);
    return end - begin;
}

static unsigned sprintl_album_info(const dr_log_printer_t* self, track_t* track, char* begin)
{
    return sprint_album_info(self, track, begin, '\n');
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

unsigned sprintl_album_header_dr_log_printer(const dr_log_printer_t* self, track_t* track, char* begin)
{
    char* end = begin;
    end += sprintl_line(self->line_length, end);
    end += sprintl_album_info(self, track, end);
    end += sprintl_line(self->line_length, end);
    end += sprintl_line(0, end);
    end += sprintl_col_headers_dr_log_printer(self, end);
    end += sprintl_line(self->line_length, end);
    return end - begin;
}

unsigned sprint_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, track_t* track, char* begin, char endline)
{
    (void) self;
    char* end = begin;
    end += sprintf(end, "Number of tracks:  %i\n", items);
    end += sprintf(end, "Official DR value: DR%.0f", avg_dr);
    if(self->sprint_codec_info)
    {
        end += sprintl_line(0, end);
        end += sprintl_line(0, end);
        end += self->sprint_codec_info(track, end);
    }
    end += sprintf(end, "%c", endline);
    return end - begin;
}

unsigned sprintl_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, track_t* track, char* begin)
{
    return sprint_summary_info(self, items, avg_dr, track, begin, '\n');
}

static unsigned sprintl_album_footer_dr_log_printer(const dr_log_printer_t* self, sample_mean_t* dr_mean, track_t* track, char* begin)
{
    char* end = begin;
    end += sprintl_line(self->line_length, end);
    end += sprintl_line(0, end);
    end += sprintl_summary_info(self, dr_mean->samples, get_sample_mean(dr_mean), track, end);
    end += sprintl_n('=', self->line_length, end);
    return end - begin;
}

unsigned sprint_log_dr_log_printer(const dr_log_printer_t* self, const tagged_dr_data_t* tagged_dr_data, char* begin)
{
    char* end = begin;
    sample_mean_t dr_mean = make_sample_mean();
    track_t* last_track = tagged_dr_data->data->item;
    end += sprintl_album_header_dr_log_printer(self, last_track, end);
    for(unsigned k = 0; k < tagged_dr_data->items; ++k)
    {
        tagged_dr_datum_t* datum = tagged_dr_data->data + k;
        int new_album = !self->same_album(last_track, datum->item);
        if(new_album)
        {
            end += sprintl_album_footer_dr_log_printer(self, &dr_mean, last_track, end);
            reset_sample_mean(&dr_mean);
            end += sprintl_album_header_dr_log_printer(self, datum->item, end);
        }
        end += sprintl_item_dr_log_printer(self, datum, end);
        observe_sample_mean(&dr_mean, datum->dr_stats.dr);
        last_track = datum->item;
    }
    end += sprintl_album_footer_dr_log_printer(self, &dr_mean, last_track, end);
    return end - begin;
}
