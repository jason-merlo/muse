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
#define SMA_LONG_LENGTH 100
#define SMA_SHORT_LENGTH 5

class Beat_Detection {
    public:
        Beat_Detection();

        bool beat_on_bin(int bin);
        bool flip();

        int r();
        int g();
        int b();
        int num_beats();

        void frame_ticked();
        void tick(audio_bins* bins);
        void set_beats_per_flip(int beats);

    private:
        void tick_beat_detection(audio_bins * bins, int cur_bin);
        void tick_bpm_detection(audio_bins * bins);

        bool beat_on;
        bool beat_reporter;
        bool flip_on;

        int red, green, blue;
        int beat_count;
        int beats_per_flip;
        int flip_count;
        int sma_long_index;
        int sma_short_index;

        float sma_long_bins[NUM_BINS][SMA_LONG_LENGTH];
        float sma_short_bins[NUM_BINS][SMA_SHORT_LENGTH];
        float sma_long_totals[NUM_BINS];
        float sma_short_totals[NUM_BINS];
        bool beat_on_bins[NUM_BINS];
        bool beat_reporter_bins[NUM_BINS];

        float bpm_long_values[SMA_LONG_LENGTH];
        float bpm_short_values[SMA_SHORT_LENGTH];
        float bpm_long_total;
        float bpm_short_total;
};

#endif // BEAT_DETECTION_H
