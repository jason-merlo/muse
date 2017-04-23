#ifndef PONG_H
#define PONG_H

#include "visualizer.h"
#include "pong_ball.h"
#include "pong_paddle.h"

class Pong : public Visualizer {
    public:
        Pong();
        Pong(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;

        int last_beat_count;

        PongPaddle * pongPaddles[2];
        PongBall * pongBall;
};

#endif
