/**************************************************************************
* @file dr_log_printer.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 15 Jun 2020
* @brief String buffer printing functions to build DR logs
* ************************************************************************/

#ifndef DR_LOG_PRINTER_H
#define DR_LOG_PRINTER_H

struct thread_datum_s;
struct thread_data_s;

struct dr_log_printer_s
{
    const char* dr_format;
    unsigned (*sprint_track_info)(void* track, char* begin);
    unsigned (*sprint_album_info)(void* first_track, char* begin);
};

typedef struct dr_log_printer_s dr_log_printer_t;

unsigned sprint_item_dr_log_printer(const dr_log_printer_t* self, struct thread_datum_s* datum, char* begin, char endline);
unsigned sprintl_item_dr_log_printer(const dr_log_printer_t* self, struct thread_datum_s* datum, char* begin);
unsigned sprint_col_headers_dr_log_printer(const dr_log_printer_t* self, char* begin, char endline);
unsigned sprintl_col_headers_dr_log_printer(const dr_log_printer_t* self, char* begin);
unsigned sprint_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, char* begin, char endline);
unsigned sprintl_summary_info(const dr_log_printer_t* self, unsigned items, double avg_dr, char* begin);
unsigned sprint_log_dr_log_printer(const dr_log_printer_t* self, const struct thread_data_s* thread_data, char* begin);

#endif /* DR_LOG_PRINTER_H */
