#include "pong_paddle.h"

#include "muse.h"

PongPaddle::PongPaddle() {}

PongPaddle::PongPaddle(int len, float x, float y, float maxXVel, float maxYVel, unsigned int tickTime, int r, int g, int b) {
    this->len = len;
    this->x = x;
    this->y = y;
    this->maxXVel = maxXVel;
    this->maxYVel = maxYVel;
    this->xVel = maxXVel;
    this->yVel = maxYVel;

    this->setColor(r, g, b);

    last_tick = millis();
    tick_time = tickTime;
}

void PongPaddle::tick(PongBall * pb) {
    if (millis() - last_tick > tick_time) {
        last_tick = millis();

        // Control movement of paddle based on ball movement
        if ((x < pb->x && pb->xVel < 0) || (x > pb->x && pb->xVel > 0)) {
            if (y+len < pb->y) { yVel = maxYVel; }
            else if (y > pb->y) { yVel = -maxYVel; }
        } else {
            if (abs(y+len/2 - STRIP_LENGTH/2) > len/4) {
                if (y+len/2 > STRIP_LENGTH/2) { yVel = 1.0 < maxYVel ? -1.0 : -maxYVel; }
                else { yVel = 1.0 < maxYVel ? 1.0 : maxYVel; }
            } else {
                yVel = 0;
            }
        }

        // Update paddle position
        y += yVel;
        if ((y+len) >= STRIP_LENGTH) {
            y = STRIP_LENGTH-len;
            yVel = 0;
        } else if (y <= 0) {
            y = 0;
            yVel = 0;
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
