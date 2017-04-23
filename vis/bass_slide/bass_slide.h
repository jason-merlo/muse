#ifndef BASS_SLIDE_H
#define BASS_SLIDE_H

#include "visualizer.h"

class BassSlide : public Visualizer {
    public:
        BassSlide();
        BassSlide(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;

        int bass_slide_heights[NUM_BARS];
        float bass_slide_ema;
        unsigned long bass_slide_millis;
};

#endif
