#ifndef BARS_H
#define BARS_H

#include "visualizer.h"

class Bars : public Visualizer {
    public:
        Bars();
        Bars(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;
};

#endif
