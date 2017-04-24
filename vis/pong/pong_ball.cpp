#include "pong_ball.h"
#include "muse.h"

PongBall::PongBall() {}

PongBall::PongBall(int len, float velX, float velY, unsigned int tickTime, int r, int g, int b) {
    this->len = len;
    xVel = velX;
    yVel = velY;
    x = NUM_BARS / 2;
    y = BAR_LENGTH / 2;

    tick_time = tickTime;
    last_tick = millis();

    setColor(r, g, b);
}

void PongBall::tick() {
    if (millis() - last_tick > tick_time) {
        last_tick = millis();
        x += xVel;
        y += yVel;

        if (x <= 0) {
            x = 0;
            xVel = -xVel;
        } else if (x >= NUM_BARS) {
            x = NUM_BARS-1;
            xVel = -xVel;
        }

        if (y <= 0) {
            y = 0;
            yVel = -yVel;
        } else if (y+len >= BAR_LENGTH) {
            y = BAR_LENGTH-len;
            yVel = -yVel;
        }
    }
}

int PongBall::xPos() { return (int) x; }
int PongBall::yPos() { return (int) y; }

void PongBall::setColor(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
}
