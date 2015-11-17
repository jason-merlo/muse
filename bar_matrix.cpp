/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  updates:
 *  11/17/2015
 *
 *  File: bar_matrix.cpp
 *
 *  Description: Class to control LED bar matrix and run visualizers
 * ================================================================== */

#include "bar_matrix.h"

// Display variables
short disp_width;
short disp_height;
bool disp_orientation;
unsigned long** display;
Adafruit_NeoPixel** bars;

/* ================================================================== *
 * Bar_matrix
 *
 * num_bars - number of LED bars
 * bar_len - number of leds in each bar (strip length)
 * orientation - horizontal or vertical bars (horizontal/ladder = 0; vertical/fence = 1)
 * ================================================================== */
Bar_Matrix::Bar_Matrix(short num_bars, short bar_len, bool orientation, const char led_type, const char* pins) {
  disp_width = num_bars; //(orientation) ? num_bars : bar_len;
  disp_height = bar_len; //(orientation) ? bar_len : num_bars;

  disp_orientation = orientation;

  display = new unsigned long*[disp_width];
  for (int i = 0; i < disp_width; i++)
    display[i] = new unsigned long[disp_height];

  bars = new Adafruit_NeoPixel*[num_bars];
  for(short i = 0; i < num_bars; i++)
    bars[i] = new Adafruit_NeoPixel(bar_len, pins[i], led_type);

  init_matrix();
}

/* ================================================================== *
 * Function: init_matrix
 * Description: Initializes the matrix to off
 * ================================================================== */
void Bar_Matrix::init_matrix() {
  for (int i = 0; i < disp_width; i++) {
    // switch height and width based on orientation
    bars[i]->begin();
    bars[i]->show();
  }
}

/* ================================================================== *
 * Function: clear_matrix
 * Description: Sets all pixel values to (0, 0, 0)
 * ================================================================== */
void Bar_Matrix::clear_matrix() {
  for (int i = 0; i < disp_width; i++) {
    for (int j =0; j< disp_height; j++) {
      bars[i]->setPixelColor(j, 0);
    }
  }
}

/* ================================================================== *
 * Function: fill_matrix
 * Description: Sets all pixel values in matrix to given color value
 * Parameters: [Color]* color - color to set matrix to
 * ================================================================== */
void Bar_Matrix::fill_matrix(Color_Value* color) {
  for (int i = 0; i < disp_width; i++) {
    for (int j =0; j< disp_height; j++) {
      //bars[i]->setPixelColor(j, color->r, color->g, color-b);
    }
    bars[i]->show();
  }
}

/* ================================================================== *
 * Function: visualizer_wheel
 * Description: Sets all bars to a color while rotating through all
 *              hues in order of the rainbow
 * Parameters: [float] hue - hue value, or rotation of wheel
 *             [float] intensity - intensity of lights
 * ================================================================== */
void Bar_Matrix::visualizer_wheel(float hue, float intensity) {
  // TODO: incorporate internal hue "rotation" algorithm
  //fill_matrix(Color(hue, 1, intensity));
}

/* ================================================================== *
 * Function: fill_matrix
 * Description: Sets all pixel values to given color value
 * Parameters: [audio_bins]* bins - frequency bins read from chip
 * ================================================================== */
void Bar_Matrix::visualizer_bars(audio_bins* bins) {
  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < disp_height; j++) {
      // get bin
      int level = (i < disp_width/2) ? bins->left[i] : bins->left[(disp_width/2) - 1 - (i-disp_width/2)];
      // set bar
      if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH)) {
        //int c = wheel(level/4096, 1, .75);
        //bars[i]->setPixelColor(j, c & 8, (c>>8) & 0xFF , (c>>16) & 0xFF);
        bars[i]->setPixelColor(j, bins->left[0]/16, bins->left[1]/16, bins->left[2]/16);
      }
    }
  }
}

/* ================================================================== *
 * Function: decay
 * Description: slowly fades out matrix values
 * Parameters: [float] factor - decay factor to be multiplied by
 * ================================================================== */
void Bar_Matrix::decay(float factor) {
  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < disp_height; j++) {
      unsigned int color = bars[i]->getPixelColor(j);
      bars[i]->setPixelColor(j, (char)(color >> 16) * factor, (char)(color >> 8) * factor , (char)(color) * factor);
    }
  }
}

/* ================================================================== *
 * Function: show_all
 * Description: drives pixels in physical matrix to current values
 * ================================================================== */
void Bar_Matrix::show_all() {
  for (char i = 0; i < disp_width; i++) {
    bars[i]->show();
  }
}

/* ================================================================== *
 * NOTE: EXPERIMENTAL - SUBJECT TO CHANGE OR BE REMOVED
 * Function: wheel
 * Description: converts
 * ================================================================== */
int wheel(float h, float s, float v) {
  float r, g, b;

  int i = floor(h * 6);
  float f = h * 6 - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  switch(i % 6){
      case 0: r = v, g = t, b = p; break;
      case 1: r = q, g = v, b = p; break;
      case 2: r = p, g = v, b = t; break;
      case 3: r = p, g = q, b = v; break;
      case 4: r = t, g = p, b = v; break;
      case 5: r = v, g = p, b = q; break;
  }

  int result = char(r)<<16 | char(g)<<8 | char(b);

  return result;
}
