#ifndef CLASSIC_H
#define CLASSIC_H

#include "visualizer.h"

class Classic : public Visualizer {
    public:
        Classic();
        Classic(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        int classic_fill_in;

        float in_factor;
        float out_factor;
};

#endif
