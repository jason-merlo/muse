#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "beat_detection.h"
#include "muse.h"
#include "neopixel.h"

#define NUM_BARS 8
#define BAR_HEIGHT 70

class Visualizer {
    public:
        Visualizer() {
            this->bars  = NULL;
            this->bd    = NULL;
            this->bins  = NULL;

            this->color_table_idx = 0;
            this->last_beat_count = 0;
        };
        Visualizer(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd) {
            this->bars  = bars;
            this->bd    = bd;
            this->bins  = bins;

            this->color_table_idx = 0;
            this->last_beat_count = 0;
        };

        virtual void tick(audio_bins * bins);

        void
            clear_matrix(),
            decay(double factor),
            decay_to_rgb(double factor, int r, int g, int b),
            fill_bar(int bar, int r, int g, int b),
            fill_matrix(Color_Value color),
            init_matrix(),
            mix_pixel(unsigned char bar, unsigned short pixel, float factor, unsigned char r, unsigned char g, unsigned char b),
            show_all();

        Adafruit_NeoPixel** bars;
        audio_bins * bins;
        Beat_Detection * bd;

        int color_table_idx;
        int last_beat_count;
};

const int COLOR_TABLE2[50][3] {
  { 25,  50,  75},
  { 50, 100, 150},
  { 75, 150, 225},
  {100, 200,  46},
  {125, 250, 121},
  {150,  46, 196},
  {175,  96,  17},
  {200, 146,  92},
  {225, 196, 167},
  {250, 246, 242},
  { 21,  42,  63},
  { 46,  92, 138},
  { 71, 142, 213},
  { 96, 192,  34},
  {121, 242, 109},
  {146,  38, 184},
  {171,  88,  05},
  {196, 138,  80},
  {221, 188, 155},
  {246, 238, 230},
  { 17,  34,  51},
  { 42,  84, 126},
  { 67, 134, 201},
  { 92, 184,  22},
  {117, 234,  97},
  {142,  30, 172},
  {167,  80, 247},
  {192, 130,  68},
  {217, 180, 143},
  {242, 230, 218},
  { 13,  26,  39},
  { 38,  76, 114},
  { 63, 126, 189},
  { 88, 176,  10},
  {113, 226,  85},
  {138,  22, 160},
  {163,  72, 235},
  {188, 122,  56},
  {213, 172, 131},
  {238, 222, 206},
  {  9,  18,  27},
  { 34,  68, 102},
  { 59, 118, 177},
  { 84, 168, 252},
  {109, 218,  73},
  {134,  14, 148},
  {159,  64, 223},
  {184, 114,  44},
  {209, 164, 119},
  {234, 214, 194}
};

#endif
