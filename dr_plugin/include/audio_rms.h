#ifndef AUDIO_RMS_H
#define AUDIO_RMS_H

double get_audio_rms(double sum2, unsigned number_of_samples); //sum of squared number_of_samples sample values, assumes 0 mean

#endif /* AUDIO_RMS_H */
