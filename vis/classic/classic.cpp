#include "classic.h"

Classic::Classic() : Visualizer::Visualizer() {};

Classic::Classic(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;

    classic_fill_in = 0;
}

void Classic::tick(audio_bins* bins) {
    decay_to_rgb(out_factor, 0, 0, 10);

    if (bd->num_beats() - last_beat_count > 4) {
        color_table_idx = (color_table_idx+1) % 51;
        last_beat_count = bd->num_beats();
    }

    classic_fill_in--;
    if (classic_fill_in == 0){
        fill_bar(7, COLOR_TABLE2[color_table_idx][0], COLOR_TABLE2[color_table_idx][1], COLOR_TABLE2[color_table_idx][2]);
    }

    for (int i = 0; i < NUM_BINS; i++) {
        if (bd->beat_on_bin(i)) {
            fill_bar(i, COLOR_TABLE2[color_table_idx][0], COLOR_TABLE2[color_table_idx][1], COLOR_TABLE2[color_table_idx][2]);

            if (i == 6) {
                classic_fill_in = 3;
            }
        }
    }
}
