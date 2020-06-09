/**************************************************************************
* @file audio_rms.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 09 Jun 2020
* @brief This RMS assumes zero mean samples to be multiplied by sqrt(2).
* This means that a 0dB peaking sine-wave has -3dB RMS instead of -6dB.
* ************************************************************************/

#ifndef AUDIO_RMS_H
#define AUDIO_RMS_H

double get_audio_rms(double sum2, unsigned number_of_samples); //sum of squared number_of_samples sample values, assumes 0 mean

#endif /* AUDIO_RMS_H */
