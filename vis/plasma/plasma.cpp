#include "plasma.h"

Plasma::Plasma() : Visualizer::Visualizer() {};

Plasma::Plasma(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;
}

void Plasma::tick() {
    decay(out_factor);

    for (char i = 0; i < disp_width; i++) {
        for (short j = disp_height - 1; j > 0; j-=2) {
            // Move wave up
            unsigned int color = bars[i]->getPixelColor(j-1);
            mix_pixel(i, j, in_factor, (char)(color >> 16), (char)(color >> 8), (char)(color));
            mix_pixel(i, j+1, in_factor, (char)(color >> 16), (char)(color >> 8), (char)(color));
        }

        // Set bar levels
        int level = 0;
        switch(i) {
            case 0: level = bins->left[LEFT_160];       break;
            case 1: level = bins->left[LEFT_1000];      break;
            case 2: level = bins->left[LEFT_6250];      break;
            case 3: level = bins->left[LEFT_16000];     break;
            case 4: level = bins->right[RIGHT_16000];   break;
            case 5: level = bins->right[RIGHT_6250];    break;
            case 6: level = bins->right[RIGHT_1000];    break;
            case 7: level = bins->right[RIGHT_160];     break;
        }

        level *= FREQ_GAIN;
        // set bar
        float val = level*2*PI/BINS_MAX;
        //float intensity = (pow((float)(level)/(float)(BINS_MAX), 2))*255;
        float intensity = (level)/(BINS_MAX)*255.0f;
        intensity *= intensity;
        if (intensity > 255) intensity = 255.0;
        mix_pixel(i, 0, in_factor, cos(val - 4*PI/3)*intensity, cos(val - 2*PI/3)*intensity, cos(val)*intensity);

        //bars[i]->setPixelColor(0, cos(val - 2*PI/3)*intensity, cos(val)*intensity, cos(val - 4*PI/3)*intensity);

    }
}
