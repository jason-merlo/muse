#ifndef PULSE_H
#define PULSE_H

#include "visualizer.h"

class Pulse : public Visualizer {
    public:
        Pulse();
        Pulse(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;
};

#endif
