#include "mesh.h"

Mesh::Mesh() : Visualizer::Visualizer() {};

Mesh::Mesh(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;

    bar1 = new MeshBar(56, 8, 0, 1.0, 0.1);// len, bin_len, movement_bin, max_speed, min_speed)
}

void Mesh::tick() {
    decay(out_factor);

    int seg_len = 10;

    bar1->tick(this->bins);

    for (int bar = 0; bar < NUM_BARS; bar++) {
        int idx = 0;

        for (int p = bar1->y; p < BAR_LENGTH && idx < bar1->len; p++) {
            mix_pixel(bar, p, in_factor, bar1->r[idx], bar1->g[idx], bar1->b[idx]);

            idx++;
        }
    }
/*
    for (int bin = 0; bin < NUM_BINS; bin++) {

        int level = bins->left[bin];

        float val = ((float)level) / BINS_MAX;
        val *= PI * 2;

        int red     = pow(cos(val - 4*PI/3  ), 2) * 128;
        int green   = pow(cos(val - 2*PI/3  ), 2) * 128;
        int blue    = pow(cos(val           ), 2) * 128;

        for (int x = 0; x < NUM_BARS; x++) {

            for (int y = seg_len *0* (bin + x%2); y < BAR_LENGTH; y += seg_len*NUM_BINS) {

                for (int i = 0; i < seg_len; i++) {
                    mix_pixel(x, y+i, in_factor, red, green, blue);
                }
            }
        }
    }
    */
}
