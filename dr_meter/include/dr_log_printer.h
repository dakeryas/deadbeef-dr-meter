/**************************************************************************
* @file dr_log_printer.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 15 Jun 2020
* @brief String buffer printing functions to build DR logs
* ************************************************************************/

#ifndef DR_METER_DR_LOG_PRINTER_H
#define DR_METER_DR_LOG_PRINTER_H

struct track_s;
struct tagged_dr_datum_s;
struct tagged_dr_data_s;

struct dr_log_printer_s
{
    const char* dr_format;
    unsigned line_length;
    unsigned (*sprint_track_info)(struct track_s* track, char* begin);
    unsigned (*sprint_album_info)(struct track_s* track, char* begin);
    int (*same_album)(struct track_s* track1, struct track_s* track2);
};

typedef struct dr_log_printer_s dr_log_printer_t;

unsigned sprint_item_dr_log_printer(const dr_log_printer_t* self, struct tagged_dr_datum_s* datum, char* begin, char endline);
unsigned sprintl_item_dr_log_printer(const dr_log_printer_t* self, struct tagged_dr_datum_s* datum, char* begin);
unsigned sprint_col_headers_dr_log_printer(const dr_log_printer_t* self, char* begin, char endline);
unsigned sprintl_col_headers_dr_log_printer(const dr_log_printer_t* self, char* begin);
unsigned sprint_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, char* begin, char endline);
unsigned sprintl_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, char* begin);
unsigned sprint_log_dr_log_printer(const dr_log_printer_t* self, const struct tagged_dr_data_s* tagged_dr_data, char* begin);

#endif /* DR_METER_DR_LOG_PRINTER_H */
