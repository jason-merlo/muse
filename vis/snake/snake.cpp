#include "snake.h"

Snake::Snake() : Visualizer::Visualizer() {};

Snake::Snake(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;

    snakes[0] = SnakeStruct(1, 30, -30, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;
    snakes[1] = SnakeStruct(-1, 25, 4*70, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;
    snakes[2] = SnakeStruct(1, 20, 185, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;
    snakes[3] = SnakeStruct(-1, 15, 8*70, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;

    snakes[0].posTicks = 0;
    snakes[0].ticksNeeded = 1;
    snakes[1].posTicks = 0;
    snakes[1].ticksNeeded = 1;
    snakes[2].posTicks = 0;
    snakes[2].ticksNeeded = 1;
    snakes[3].posTicks = 0;
    snakes[3].ticksNeeded = 1;
}

void Snake::tick() {
    bool noChange = true;

    fill_matrix(Color_Value(0, 0, 0));

    for (int x = 0; x < NUM_SNAKES; x++) {
        SnakeStruct *s = &snakes[x];

        for (int i = 0; i < s->len; i++) {
            int b = (s->pos + i) / 70; // bar
            int p = (s->pos + i) % 70; // pixel

            if (b % 2 == 1) {
                p = 69-p;
            }

            mix_pixel(b, p, .95, s->r, s->g, s->b);
        }

        if (noChange && random(0, 10) > 7) {
            s->r = bd->r();
            s->g = bd->g();
            s->b = bd->b();
            noChange = false;
        }

        s->posTicks++;
        if (s->posTicks >= s->ticksNeeded) {
            s->posTicks = 0;
            s->pos += s->dir;
        }
        if ((s->dir > 0 && s->pos > 8*70+s->len) ||
            (s->dir < 0 && s->pos < -s->len))
        {
            color_table_idx += random(0, 10);
            color_table_idx = color_table_idx % 50;

            s->pos = s->dir > 0 ? -s->len : 8*70;
        }
    }
}
