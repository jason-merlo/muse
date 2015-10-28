#ifndef BAR_MATRIX_H
#define BAR_MATRIX_H

#include "application.h"
#include "neopixel.h"

// Define bar orientations
#define HORIZONTAL  0
#define VERTICAL    1

class Bar_Matrix {
  public:
    Bar_Matrix(short num_bars, short bar_len, bool orientation, char led_type, const char* pins);

    void
      update_matrix();

    unsigned long**
      get_instance();

  private:
  void
    init_matrix();
};

#endif // BAR_MATRIX_H
