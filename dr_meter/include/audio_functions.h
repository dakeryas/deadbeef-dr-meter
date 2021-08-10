/**************************************************************************
* @file audio_functions.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief dB function
* ************************************************************************/

#ifndef DR_METER_AUDIO_FUNCTIONS_H
#define DR_METER_AUDIO_FUNCTIONS_H

double decibels(double value);
/* This RMS assumes zero mean samples to be multiplied by sqrt(2).
   This means that a 0dB peaking sine-wave has -3dB RMS instead of -6dB.*/
double get_audio_rms(double sum2, unsigned number_of_samples);
/* Return value such that fabs(value) <= 1 */
double true_peak(double peak_value);

#endif /* DR_METER_AUDIO_FUNCTIONS_H */
