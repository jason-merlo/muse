#include "bass_slide.h"

BassSlide::BassSlide() : Visualizer::Visualizer() {};

BassSlide::BassSlide(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;

    bass_slide_ema = 0.0;
    bass_slide_millis = millis();
}

void BassSlide::tick() {
    decay(out_factor);

    if (bd->num_beats() != last_beat_count) {
        last_beat_count = bd->num_beats();
        color_table_idx++;
        color_table_idx %= 51;

        //tcpBeats.stevenSendRGB(COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    }

    // Average all 4 low frequency bins
    float energy = (bins->right[RIGHT_63] + bins->left[LEFT_63]) + (bins->right[RIGHT_160] + bins->left[LEFT_160])+ (bins->right[RIGHT_400] + bins->left[LEFT_400])+ (bins->right[RIGHT_1000] + bins->left[LEFT_1000])+ (bins->right[RIGHT_2500] + bins->left[LEFT_2500])+ (bins->right[RIGHT_6250] + bins->left[LEFT_6250])+ (bins->right[RIGHT_16000] + bins->left[LEFT_16000]);
    energy = energy / 2;
    bass_slide_ema = 0.5 * energy + 0.5 * bass_slide_ema;
    energy = bass_slide_ema;

    energy /= 4.0;
    energy /= (float) BINS_MAX;
    energy *= energy;
    // Use the "energy" of the low frequency bins to determine number of LEDs to light
    // Add 0.5 forces round to nearest integer
    //bass_slide_ema = 0.9 * energy + 0.1 * bass_slide_ema;
    int max_lit = (energy * (float)BAR_LENGTH) + 0.5;
    int middle_led = BAR_LENGTH / 2;

    if (millis() - bass_slide_millis > 35) {
        bass_slide_millis = millis();

        bass_slide_heights[0] = bass_slide_heights[1];
        bass_slide_heights[1] = bass_slide_heights[2];
        bass_slide_heights[2] = bass_slide_heights[3];
        bass_slide_heights[3] = bass_slide_heights[4];
        bass_slide_heights[4] = max_lit;
    }

    for (int x = 0; x < NUM_BARS / 2; x++) {
        int num_lit = bass_slide_heights[x];// / (float)(NUM_BARS/2 - x);
        int y = (BAR_LENGTH / 2) - (num_lit / 2);

        for (int i = 0; i < num_lit; i++) {
            mix_pixel(x, y+i, in_factor, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
            mix_pixel(NUM_BARS-x-1, y+i, in_factor, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
        }
    }
}
