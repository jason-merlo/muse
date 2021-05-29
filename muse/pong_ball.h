#ifndef PONG_BALL_H_
#define PONG_BALL_H_

class PongBall {

    public:
        PongBall();
        PongBall(int len, float velX, float velY, unsigned int tickTime, int r, int g, int b);

        int xPos();
        int yPos();

        void tick();
        void setColor(int r, int g, int b);

        float maxXVel;
        float maxYVel;
        float xVel;
        float yVel;
        float x;
        float y;
        int len;
        unsigned int last_tick;
        unsigned int tick_time;

        int r;
        int g;
        int b;
};

#endif
