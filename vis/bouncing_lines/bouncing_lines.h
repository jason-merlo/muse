#ifndef BOUNCING_LINES_H
#define BOUNCING_LINES_H

#include "visualizer.h"

class BouncingLines : public Visualizer {
    public:
        BouncingLines();
        BouncingLines(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;

        int bouncing_line_lengths[NUM_BARS];    // Hold the lengths of the bars
        float bouncing_line_positions[NUM_BARS];// Hold the position of the bottom of the bouncing lines
        float bouncing_line_speeds[NUM_BARS];   // Hold the speeds of teh bouncing lines
        int bouncing_line_directions[NUM_BARS]; // Hold the direction each line is moving
        int bouncing_line_colors[NUM_BARS][3];
};

#endif
