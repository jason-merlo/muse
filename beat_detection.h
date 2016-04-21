/* ================================================================== *
*  Written by Steven Ploog
*
*  updates:
*
*  File: beat_detection.h
*
*  Description: Class declarations for beat detection
* ================================================================== */

#ifndef BEAT_DETECTION_H
#define BEAT_DETECTION_H

#include "muse.h"

// Constants
#define SMA_LONG_LENGTH 450
#define SMA_SHORT_LENGTH 10

class Beat_Detection {
    public:
        Beat_Detection();

        bool flip();

        int r();
        int g();
        int b();
        int num_beats();

        void tick(audio_bins* bins);
        void set_beats_per_flip(int beats);

    private:
        bool beat_on;
        bool flip_on;

        int red, green, blue;
        int beat_count;
        int beats_per_flip;
        int flip_count;
        int sma_long_index;
        int sma_short_index;

        float sma_long_values[SMA_LONG_LENGTH];
        float sma_short_values[SMA_SHORT_LENGTH];
        float sma_long_total;
        float sma_short_total;
};

#endif // BEAT_DETECTION_H
