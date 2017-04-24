#include "pong.h"

Pong::Pong() : Visualizer::Visualizer() {};

Pong::Pong(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;

    last_beat_count = 0;

    pongPaddles[0]  = new PongPaddle(15, 0, BAR_LENGTH/2, 0, 0.9, 15, 100, 100, 100);
    pongPaddles[1]  = new PongPaddle(15, NUM_BARS-1, BAR_LENGTH/2, 0, 0.9, 15, 100, 100, 100);
    pongBall        = new PongBall(3, 0.0625, 0.80, 15, 100, 100, 100);
}

void Pong::tick() {
    pongPaddles[0]->tick(pongBall);
    pongPaddles[1]->tick(pongBall);
    pongBall->tick();

    if (bd->num_beats() != last_beat_count) {
        last_beat_count = bd->num_beats();
        if (random(0, 3) == 0 ) {
            pongPaddles[1]->setColor(bd->r(), bd->g(), bd->b());
        } else if (random(0, 2) == 0) {
            pongPaddles[0]->setColor(bd->r(), bd->g(), bd->b());
        } else {
            pongBall->setColor(bd->r(), bd->g(), bd->b());
        }
    }

    clear_matrix();

    // Draw paddles
    for (int i = 0; i < 2; i++) {
        for (int y = 0; y < pongPaddles[i]->len; y++) {
            mix_pixel(pongPaddles[i]->xPos(), pongPaddles[i]->yPos()+y, in_factor, pongPaddles[i]->r, pongPaddles[i]->g, pongPaddles[i]->b);
        }
    }

    // Draw Ball
    for (int i = 0; i < pongBall->len; i++) {
        mix_pixel(pongBall->xPos(), pongBall->yPos()+i, in_factor, pongBall->r, pongBall->g, pongBall->b);
    }
}
