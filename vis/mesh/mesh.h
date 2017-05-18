#ifndef MESH_H
#define MESH_H

#include "visualizer.h"

#include "mesh_bar.h"

class Mesh : public Visualizer {
    public:
        Mesh();
        Mesh(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor);

        void tick();

        float in_factor;
        float out_factor;

        MeshBar * bar1;
};

#endif
