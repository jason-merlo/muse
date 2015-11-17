#ifndef BAR_MATRIX_H
#define BAR_MATRIX_H

#include "application.h"
#include "neopixel.h"
#include "muse.h"

#include "math.h"

// Define bar orientations
#define HORIZONTAL  0
#define VERTICAL    1

class Bar_Matrix {
  public:
    Bar_Matrix(short num_bars, short bar_len, bool orientation, char led_type, const char* pins);

    void
      clear_matrix(),
      fill_matrix(Color_Value* color),
      visualizer_wheel(float WheelPos, float intensity),
      visualizer_bars(audio_bins* bins),
      show_all(),
      decay(float factor);

    int
      wheel(float h, float s, float v);

  private:
  void
    init_matrix();
};

#endif // BAR_MATRIX_H
