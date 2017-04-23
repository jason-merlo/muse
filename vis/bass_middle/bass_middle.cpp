#include "bass_middle.h"

BassMiddle::BassMiddle() : Visualizer::Visualizer() {};

BassMiddle::BassMiddle(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;
}

void BassMiddle::tick() {
    decay(out_factor);

    if (bd->num_beats() != last_beat_count) {
        last_beat_count = bd->num_beats();
        color_table_idx++;
        color_table_idx %= 51;

        //tcpBeats.stevenSendRGB(COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    }

    // Average all 4 low frequency bins
    float energy = bins->right[RIGHT_63] + bins->left[LEFT_63] + bins->right[RIGHT_160] + bins->left[LEFT_160];
    energy /= 4.0;
    energy /= (float) BINS_MAX;
    energy *= energy;
    // Use the "energy" of the low frequency bins to determine number of LEDs to light
    // Add 0.5 forces round to nearest integer
    int max_lit = (energy * (float)STRIP_LENGTH) + 0.5;
    int middle_led = STRIP_LENGTH / 2;

    for (int x = 0; x < NUM_BARS / 2; x++) {
        int num_lit = max_lit / (NUM_BARS/2 - x);
        int y = (STRIP_LENGTH / 2) - (num_lit / 2);

        for (int i = 0; i < num_lit; i++) {
            mix_pixel(x, y+i, in_factor, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
            mix_pixel(NUM_BARS-x-1, y+i, in_factor, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
        }
    }
}
