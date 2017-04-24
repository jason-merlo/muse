#include "pulse.h"

Pulse::Pulse() : Visualizer::Visualizer() {};

Pulse::Pulse(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;
}

void Pulse::tick() {
    decay (out_factor);

    float distance_x = 1.0f;
    float distance_y = 20.0f;

    // Calculate frequency pan and "fade"
    float pans[NUM_BINS]; // ratio left to right, 0-1
    float intensities[NUM_BINS]; // ratio low to high, 0-1
    for (char i = 0; i < NUM_BINS; i++)
    pans[i] = float(bins->left[i])/float(bins->right[i]);
    for (char i = 0; i < NUM_BINS; i++)
    intensities[i] = ((float(bins->left[i])+float(bins->right[i]))/2.0f)/BINS_MAX;

    for (char i = 0; i < NUM_BARS; i++) {
        for (char j = 0; j < BAR_LENGTH; j++) {
            float level = 0;

            // Calculate level intensity
            for (char x = 0; x < NUM_BINS; x++) {
                float distance = distance_x * 1.0f/(abs(pans[x]-(i/NUM_BARS))) + distance_y * 1.0f/(abs((x/NUM_BINS)-(j/BAR_LENGTH)));
                level += intensities[x] * distance;
            }

            //level *= 2*PI;
            //mix_pixel(i, j, in_factor, cos(level)*255*level, cos(level - 2*PI/3)*255*level, cos(level - 4*PI/3)*255*level);
            mix_pixel(i, j, in_factor, level*255, level*255, level*255);
        }
    }
}
