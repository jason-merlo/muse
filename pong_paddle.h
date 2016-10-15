#ifndef PONG_PADDLE_H_
#define PONG_PADDLE_H_

#include "pong_ball.h"

class PongPaddle {

    public:
        PongPaddle();
        PongPaddle(int len, float xPos, float yPos, float xVel, float yVel, unsigned int tickTime, int r, int g, int b);

        void tick(PongBall * pb);
        void setColor(int r, int g, int b);

        int xPos();
        int yPos();

        int len;
        float x;
        float y;
        float xVel;
        float yVel;
        float maxXVel;
        float maxYVel;
        int last_tick;
        int tick_time;

        int r;
        int g;
        int b;
};

#endif
