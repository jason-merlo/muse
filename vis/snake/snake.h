#ifndef SNAKE_H
#define SNAKE_H

#include "visualizer.h"

#define NUM_SNAKES 4

struct SnakeStruct {

    int dir;
    int len;
    int pos;
    int posTicks;
    int ticksNeeded;

    int r;
    int g;
    int b;

    SnakeStruct(int dir, int len, int pos, int r, int g, int b) :
        dir(dir), len(len), pos(pos), r(r), g(g), b(b) {}

    SnakeStruct() {}
};

class Snake : public Visualizer {
    public:
        Snake();
        Snake(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;

        SnakeStruct snakes[NUM_SNAKES];
};

#endif
