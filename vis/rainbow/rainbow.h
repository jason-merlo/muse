#ifndef RAINBOW_H
#define RAINBOW_H

#include "visualizer.h"

class Rainbow : public Visualizer {
    public:
        Rainbow();
        Rainbow(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;
};

#endif
