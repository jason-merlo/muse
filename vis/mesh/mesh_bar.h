#ifndef MESH_BAR_H
#define MESH_BAR_H

#include "visualizer.h"

class MeshBar {
    public:
        MeshBar();
        MeshBar(int len, int bin_len, int movement_bin, float max_speed, float min_speed);

        void tick(audio_bins * bins);

        int bin_len;
        int direction;
        int len;
        int movement_bin;
        int * r;
        int * g;
        int * b;

        float y;
        float speed;
        float max_speed;
        float min_speed;
};

#endif
