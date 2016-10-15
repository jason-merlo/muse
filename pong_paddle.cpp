#include "pong_paddle.h"

#include "muse.h"

PongPaddle::PongPaddle() {}

PongPaddle::PongPaddle(int len, float x, float y, float xVel, float yVel, unsigned int tickTime, int r, int g, int b) {
    this->len = len;
    this->x = x;
    this->y = y;
    this->xVel = xVel;
    this->yVel = yVel;

    this->setColor(r, g, b);

    last_tick = millis();
    tick_time = tickTime;
}

void PongPaddle::tick() {
    if (millis() - last_tick > tick_time) {
        last_tick = millis();

        y += yVel;
        if (y > STRIP_LENGTH || y < 0) {
            yVel = -yVel;
        }
    }
}

int PongPaddle::xPos() { return (int) x; }
int PongPaddle::yPos() { return (int) y; }

void PongPaddle::setColor(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
}
