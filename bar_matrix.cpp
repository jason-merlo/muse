#include "bar_matrix.h"

// Display variables
short disp_width;
short disp_height;
bool disp_orientation;
unsigned long** display;
Adafruit_NeoPixel** bars;

/*
 * Bar_matrix
 *
 * num_bars - number of LED bars
 * bar_len - number of leds in each bar (strip length)
 * orientation - horizontal or vertical bars (horizontal/ladder = 0; vertical/fence = 1)
 */
Bar_Matrix::Bar_Matrix(short num_bars, short bar_len, bool orientation, char led_type, const char* pins) {
  disp_width = (orientation) ? num_bars : bar_len;
  disp_height = (orientation) ? bar_len : num_bars;

  disp_orientation = orientation;

  display = new unsigned long*[disp_width];
  for(short i = 0; i < disp_width; ++i)
    display[i] = new unsigned long[disp_height];

  for(short i = 0; i < num_bars; i++)
    bars[i] = new Adafruit_NeoPixel(bar_len, pins[i], led_type);

  init_matrix();
}

/*
 * init_matrix
 *
 * Initializes the matrix to off
 */
void Bar_Matrix::init_matrix() {
  for (int i = 0; i < disp_width; i++) {
    for (int j =0; j< disp_height; j++) {
      // switch height and width based on orientation
      bars[i]->begin();
    }
    bars[i]->show();
  }
}

/*
 * update_matrix
 *
 * Drives the matrix to match the current values in the display matrix
 */
void Bar_Matrix::update_matrix() {
  for (int i = 0; i < disp_width; i++) {
    for (int j =0; j< disp_height; j++) {
      // switch height and width based on orientation
      bars[i]->setPixelColor(j, display[(disp_orientation) ? i : j][(disp_orientation) ? j : i]);
    }
    bars[i]->show();
  }
}

/*
 * get_instance
 *
 * returns array pointer to current bar matrix display
 */
unsigned long** Bar_Matrix::get_instance() {
  return display;
}
