#include "mesh_bar.h"

#define PADDING 10

MeshBar::MeshBar() {};

MeshBar::MeshBar(int len, int bin_len, int movement_bin, float max_speed, float min_speed)
{
    this->bin_len = bin_len;
    this->len = len;
    this->movement_bin = movement_bin;
    this->r = new int[len];
    this->g = new int[len];
    this->b = new int[len];

    this->direction = 1;

    this->y = 0.0;
    this->speed = 0.0;
    this->max_speed = max_speed;
    this->min_speed = min_speed;
}

void MeshBar::tick(audio_bins * bins) {

    for (int i = 0; i < NUM_BINS; i++) {

        float val = ((float)bins->left[i]) / BINS_MAX;
        val *= PI * 2;

        int red     = pow(cos(val - 4*PI/3  ), 2) * 128;
        int green   = pow(cos(val - 2*PI/3  ), 2) * 128;
        int blue    = pow(cos(val           ), 2) * 128;

        for (int y = 0; y < this->len; y++) {
            if (i == (y % (this->bin_len * NUM_BINS)) / this->bin_len) {
                this->r[y] = red;
                this->g[y] = green;
                this->b[y] = blue;
            }
        }

        // Update speed/position if we are in the proper bin
        if (i == this->movement_bin) {
            this->speed += .25*((float)bins->left[i] - 0.5*BINS_MAX);
            if (this->speed > this->max_speed) {
                this->speed = this->max_speed;
            } else if (this->speed < this->min_speed) {
                this->speed = this->min_speed;
            }

            this->y += this->direction * this->speed;
            if (this->y > BAR_LENGTH + PADDING - this->len) {
                this->direction *= -1;
                this->y = BAR_LENGTH + PADDING - this->len;
            } else if (this->y < -PADDING) {
                this->direction *= -1;
                this->y = -PADDING;
            }
        }
    }
}
