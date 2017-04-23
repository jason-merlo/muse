#include "bars_middle.h"

BarsMiddle::BarsMiddle() : Visualizer::Visualizer() {};

BarsMiddle::BarsMiddle(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;
}

void BarsMiddle::tick() {
    decay(out_factor);

    if (bd->beat_on_bin(0) || bd->beat_on_bin(1)) {
        //fill_bar(i, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
        color_table_idx = (color_table_idx+1) % 51;
    }

    // Right bins, grows downwards
    for (char i = 0; i < NUM_BARS; i++) {
        for (char j = 0; j < BAR_HEIGHT/2; j++) {
            // get bin
            // Set bar levels
            int level = 0;
            switch(i) {
                case 0: level = bins->left[LEFT_63];    break;
                case 1: level = bins->left[LEFT_160];   break;
                case 2: level = bins->left[LEFT_400];   break;
                case 3: level = bins->left[LEFT_1000];  break;
                case 4: level = bins->left[LEFT_2500];  break;
                case 5: level = bins->left[LEFT_6250];  break;
                case 6: level = bins->left[LEFT_1000];  break;
                case 7: level = bins->left[LEFT_16000]; break;
            }
            level *= FREQ_GAIN;
            // set bar
            float p = (float)(level)/(float)(BINS_MAX);
            if (j < p*p * (BAR_HEIGHT/2)) {
                //j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (BAR_HEIGHT/2)) {
                float val = level*2*PI/BINS_MAX;
                //mix_pixel(i, BAR_HEIGHT/2 - j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
                //mix_pixel(i, BAR_HEIGHT/2 - j, in_factor, bd->r(), bd->g(), bd->b());
                mix_pixel(i, BAR_HEIGHT/2 - j, in_factor, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
            }
        }

        // Left bins, grow upwards
        for (char j = BAR_HEIGHT/2; j < BAR_HEIGHT; j++) {
            // get bin
            int level = 0;
            switch(i) {
                case 0: level = bins->right[RIGHT_63];    break;
                case 1: level = bins->right[RIGHT_160];   break;
                case 2: level = bins->right[RIGHT_400];   break;
                case 3: level = bins->right[RIGHT_1000];  break;
                case 4: level = bins->right[RIGHT_2500];  break;
                case 5: level = bins->right[RIGHT_6250];  break;
                case 6: level = bins->right[RIGHT_1000];  break;
                case 7: level = bins->right[RIGHT_16000]; break;
            }
            level *= FREQ_GAIN;
            // set bar
            float p = (float)(level)/(float)(BINS_MAX);
            if (j-BAR_HEIGHT/2 < p*p * (BAR_HEIGHT/2)) {
                //j-BAR_HEIGHT/2 < (pow((float)(level)/(float)(BINS_MAX), 2)) * (BAR_HEIGHT/2)) {
                float val = level*2*PI/BINS_MAX;
                //mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
                //mix_pixel(i, j, in_factor, bd->r(), bd->g(), bd->b());//reds[i], greens[i], blues[i]);
                mix_pixel(i, j, in_factor, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
            }
        }
    }
}
