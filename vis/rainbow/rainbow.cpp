#include "rainbow.h"

Rainbow::Rainbow() : Visualizer::Visualizer() {};

Rainbow::Rainbow(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;
}

void Rainbow::tick() {
    decay(out_factor);

    for (char i = 0; i < NUM_BARS/2; i++) {
        int led_index = 0;
        for (char j = 0; j < BAR_LENGTH; j+=10) {
            // get bin
            int level = bins->left[j/10];
            level *= FREQ_GAIN;
            // set bar
            if (i < (pow((float)(level)/(float)(BINS_MAX), 2)) * (NUM_BARS/2)) {
                float val = level*PI/BINS_MAX;
                for (int x = 0; x < 10; x++) {
                    //mix_pixel((NUM_BARS/2)-i-1, x*NUM_BARS+led_index, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
                    mix_pixel((NUM_BARS/2)-i-1, x*NUM_BARS+led_index, in_factor, bd->r(), bd->g(), bd->b());
                }
            }

            led_index++;
            led_index = led_index%10;
        }
    }

    for (char i = NUM_BARS/2; i < NUM_BARS; i++) {
        int led_index = 0;
        for (char j = 0; j < BAR_LENGTH; j+=10) {
            // get bin
            int level = bins->right[j/10];
            level *= FREQ_GAIN;
            // set bar
            if (i-NUM_BARS/2 < (pow((float)(level)/(float)(BINS_MAX), 2)) * (NUM_BARS/2)) {
                float val = level*PI/BINS_MAX;
                for (int x = 0; x < 10; x++) {
                    //mix_pixel(i, x*NUM_BARS+led_index, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
                    mix_pixel(i, x*NUM_BARS+led_index, in_factor, bd->r(), bd->g(), bd->b());
                }
            }

            led_index++;
            led_index = led_index%10;
        }
    }

    for (char i = 0; i < NUM_BARS; i++) {
        for (char j = 0; j < BAR_LENGTH/4; j++) {
            // get bin
            int level = 0;//bins->left[i%(NUM_BARS-1)];
            switch(i) {
                case 0:
                level = bins->left[LEFT_63]; //1kHz
                break;
                case 1:
                level = bins->left[LEFT_160]; //6.25kHz
                break;
                case 2:
                level = bins->left[LEFT_400]; //63Hz
                break;
                case 3:
                level = bins->left[LEFT_1000]; //400Hz
                break;
                case 4:
                level = bins->left[LEFT_2500]; //400Hz
                break;
                case 5:
                level = bins->left[LEFT_6250]; //63Hz
                break;
                case 6:
                level = bins->left[LEFT_1000]; //6.25kHz
                break;
                case 7:
                level = bins->left[LEFT_16000]; //1kHz
                break;
            }
            level *= FREQ_GAIN;

            // set bar
            if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (BAR_LENGTH/2)) {
                float val = level*2*PI/BINS_MAX;
                mix_pixel(i, BAR_LENGTH/2 - j, .95, 255-bd->r(), 255-bd->g(), 255-bd->b());
            }
        }

        // Left bins, grow upwards
        for (char j = BAR_LENGTH/2; j < BAR_LENGTH*0.75f; j++) {
            // get bin
            int level = 0;
            switch(i) {
                case 0:
                level = bins->right[LEFT_63]; //1kHz
                break;
                case 1:
                level = bins->right[LEFT_160]; //6.25kHz
                break;
                case 2:
                level = bins->right[LEFT_400]; //63Hz
                break;
                case 3:
                level = bins->right[LEFT_1000]; //400Hz
                break;
                case 4:
                level = bins->right[LEFT_2500]; //400Hz
                break;
                case 5:
                level = bins->right[LEFT_6250]; //63Hz
                break;
                case 6:
                level = bins->right[LEFT_1000]; //6.25kHz
                break;
                case 7:
                level = bins->right[LEFT_16000]; //1kHz
                break;
            }
            level *= FREQ_GAIN;
            // set bar
            float p = (float)(level)/(float)(BINS_MAX);
            if (j-BAR_LENGTH/2 < (pow((float)(level)/(float)(BINS_MAX), 2)) * (BAR_LENGTH/2)) {
                //j-BAR_LENGTH/2 < (pow((float)(level)/(float)(BINS_MAX), 2)) * (BAR_LENGTH/2)) {
                float val = level*2*PI/BINS_MAX;
                //mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
                mix_pixel(i, j, .95, 255-bd->r(), 255-bd->g(), 255-bd->b());//reds[i], greens[i], blues[i]);
            }
        }
    }
}
