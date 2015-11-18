/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  updates:
 *  11/17/2015
 *
 *  File: bar_matrix.h
 *
 *  Description: Class declarations for bar matrix
 * ================================================================== */

#ifndef BAR_MATRIX_H
#define BAR_MATRIX_H

#include "application.h"
#include "neopixel.h"
#include "muse.h"

#include "math.h"

class Bar_Matrix {
  public:
    Bar_Matrix(short num_bars, short bar_len, char led_type, const char* pins);

    void
      clear_matrix(),
      fill_matrix(Color_Value color),
      visualizer_wheel(float WheelPos, float intensity),
      visualizer_bars(audio_bins* bins, float in_factor, float out_factor),
      show_all(),
      decay(float factor),
      mix_pixel(unsigned char bar, unsigned short pixel, float factor, unsigned char r, unsigned char g, unsigned char b);

  private:
  void
    init_matrix();
};

#endif // BAR_MATRIX_H
