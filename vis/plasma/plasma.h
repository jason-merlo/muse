#ifndef PLASMA_H
#define PLASMA_H

#include "visualizer.h"

class Plasma : public Visualizer {
    public:
        Plasma();
        Plasma(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;
};

#endif
