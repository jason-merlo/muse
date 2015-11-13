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
      purple_matrix(),
      clear_matrix(),
      visualizer_wheel(float WheelPos),
      visualizer_bars(audio_bins* bins);

      uint32_t
        Wheel(Adafruit_NeoPixel bar, byte WheelPos, float intensity);

  private:
  void
    init_matrix();
};

#endif // BAR_MATRIX_H
