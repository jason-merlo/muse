#ifndef BARS_MIDDLE_H
#define BARS_MIDDLE_H

#include "visualizer.h"

class BarsMiddle : public Visualizer {
    public:
        BarsMiddle();
        BarsMiddle(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;
};

#endif
