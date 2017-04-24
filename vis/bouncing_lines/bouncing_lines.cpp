#include "bouncing_lines.h"

BouncingLines::BouncingLines() : Visualizer::Visualizer() {};

BouncingLines::BouncingLines(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;

    for(short i = 0; i < NUM_BARS; i++) {
        // Set up variables to track bouncing lines
        bouncing_line_lengths[i] = random(10, BAR_LENGTH / 2);
        bouncing_line_positions[i] = random(0, BAR_LENGTH);
        //bouncing_line_positions[i] = (i%2 == 0) ? 0 : BAR_LENGTH;
        if (random(0, 10) > 5)  bouncing_line_directions[i] = 1;
        else                    bouncing_line_directions[i] = -1;

        bouncing_line_speeds[i] = (float)random(5, 10) / 15.0;

        bouncing_line_colors[i][0] = bd->r();
        bouncing_line_colors[i][1] = bd->g();
        bouncing_line_colors[i][2] = bd->b();
    }
}

void BouncingLines::tick() {
    for (int i = 0; i < NUM_BARS; i++) {
        int bottom = bouncing_line_positions[i];

        // Clear the pixel the line just left, light the one it entered
        if (bouncing_line_directions[i] == 1) {
            if (bottom > 0) mix_pixel(i, bottom-1, 1, 0, 0, 0);
            mix_pixel(i, 1+bottom+bouncing_line_lengths[i], 1, bouncing_line_colors[i][0], bouncing_line_colors[i][1], bouncing_line_colors[i][2]);//bd->r(), bd->g(), bd->b());
        } else {
            if (bottom > 0) mix_pixel(i, bottom-1, 1, bouncing_line_colors[i][0], bouncing_line_colors[i][1], bouncing_line_colors[i][2]);//bd->r(), bd->g(), bd->b());
            mix_pixel(i, 1+bottom+bouncing_line_lengths[i], 1, 0, 0, 0);
        }

        // Move the line
        bouncing_line_positions[i] += bouncing_line_directions[i] * bouncing_line_speeds[i];

        // Ensure we are moving in the proper direction
        if (bouncing_line_positions[i] < -.5*bouncing_line_lengths[i] && bouncing_line_directions[i] < 0) {
            bouncing_line_directions[i] = 1;
            if (random(0, 10) > 5) {
                int newlength = random(10, BAR_LENGTH / 2);
                if (newlength > bouncing_line_lengths[i]) {
                    bouncing_line_positions[i] = (-1*newlength/2) - (newlength/2 - bouncing_line_lengths[i]/2);
                } else {
                    bouncing_line_positions[i] = -1 * newlength / 2;
                }

                bouncing_line_lengths[i] = newlength;
            }

            if (random(0, 10) > 7) {
                bouncing_line_speeds[i] = (float)random(5, 10) / 15.0;
            }

            bouncing_line_colors[i][0] = bd->r();
            bouncing_line_colors[i][1] = bd->g();
            bouncing_line_colors[i][2] = bd->b();
        } else if (bouncing_line_positions[i] > BAR_LENGTH-.5*bouncing_line_lengths[i] && bouncing_line_directions[i] > 0) {
            bouncing_line_directions[i] = -1;

            bouncing_line_colors[i][0] = bd->r();
            bouncing_line_colors[i][1] = bd->g();
            bouncing_line_colors[i][2] = bd->b();
        }
    }
}
