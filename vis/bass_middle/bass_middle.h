#ifndef BASS_MIDDLE_H
#define BASS_MIDDLE_H

#include "visualizer.h"

class BassMiddle : public Visualizer {
    public:
        BassMiddle();
        BassMiddle(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;
};

#endif
