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

// constants
#define PI        3.141592

class Bar_Matrix {
  public:
    Bar_Matrix(short num_bars, short bar_len, char led_type, const char* pins);

    void
      clear_matrix(),
      fill_matrix(Color_Value color),
      bouncing_lines(float speed),
      visualizer_wheel(float intensity, float speed),
      visualizer_bars(audio_bins* bins, float in_factor, float out_factor, bool strobe),
      visualizer_bars_middle(audio_bins* bins, float in_factor, float out_factor),
      visualizer_pulse(audio_bins* bins, float in_factor, float out_factor, float decay_x, float decay_y),
      visualizer_plasma(audio_bins* bins, float in_factor, float out_factor),
      bar_test(),
      pixel_test(),
      show_all(),
      decay(double factor),
      mix_pixel(unsigned char bar, unsigned short pixel, float factor, unsigned char r, unsigned char g, unsigned char b);


  private:
    int bouncing_line_lengths[NUM_BARS];  // Hold the lengths of the bars
    float bouncing_line_positions[NUM_BARS]; // Hold the position of the bottom of the bouncing lines
    int bouncing_line_directions[NUM_BARS]; // Hold the direction each line is moving

  void
    init_matrix();
};

#endif // BAR_MATRIX_H
