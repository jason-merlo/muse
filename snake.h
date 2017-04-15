#ifndef SNAKE_H_
#define SNAKE_H_

struct Snake {

    int dir;
    int len;
    int pos;
    int posTicks;
    int ticksNeeded;

    int r;
    int g;
    int b;

    Snake(int dir, int len, int pos, int r, int g, int b) :
        dir(dir), len(len), pos(pos), r(r), g(g), b(b) {}

    Snake() {}
};

#endif
