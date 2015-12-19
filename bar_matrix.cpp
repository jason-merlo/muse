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
#include "math.h"

// Display variables
short disp_width;
short disp_height;
unsigned long** display;
Adafruit_NeoPixel** bars;

/* ================================================================== *
 * Bar_matrix
 *
 * num_bars - number of LED bars
 * bar_len - number of leds in each bar (strip length)
 * orientation - horizontal or vertical bars (horizontal/ladder = 0; vertical/fence = 1)
 * ================================================================== */
Bar_Matrix::Bar_Matrix(short num_bars, short bar_len, const char led_type, const char* pins) {
  disp_width = num_bars; //(orientation) ? num_bars : bar_len;
  disp_height = bar_len; //(orientation) ? bar_len : num_bars;

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
void Bar_Matrix::fill_matrix(Color_Value c) {
  for (int i = 0; i < disp_width; i++) {
    for (int j =0; j< disp_height; j++) {
      bars[i]->setPixelColor(j, c.c);
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
  //fill_matrix(Color_Value(hue, 1, intensity));
}

/* ================================================================== *
 * Function: fill_matrix
 * Description: Sets all pixel values to given color value
 * Parameters: [audio_bins]* bins - frequency bins read from chip
 * ================================================================== */
void Bar_Matrix::visualizer_bars(audio_bins* bins, float in_factor, float out_factor, int* bar_levels) {
  decay(out_factor);
  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < disp_height; j++) {
      // get bin
      //int level = (i < disp_width/2) ? bar_levels[i] : bar_levels[7-i];
      int level = (i < disp_width/2) ? bins->left[i+1] : bins->left[((disp_width/2) - 1 - (i-disp_width/2))+1];
      // set bar
      if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH)) {
      //if (j < (float)(level)/(float)(BINS_MAX) * (STRIP_LENGTH)) {
        const float pi = 3.141592;
        float val = level*2*pi/4096.0;
        mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*pi/3)*255, cos(val - 4*pi/3)*255);
        /*mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*pi/3)*255, cos(val - 4*pi/3)*255);*/

        /*mix_pixel(i, j, in_factor, bins->left[0]/16,
                                   bins->left[1]/16,
                                   bins->left[2]/16);*/

        /*mix_pixel(i, j, in_factor, bins->left[0]/(64-(bins->left[1]/128)),
                                   bins->left[1]/(64-(bins->left[2]/128)),
                                   bins->left[2]/(64-(bins->left[0]/128)));*/
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
      bars[i]->setPixelColor(j, (char)(color >> 16) * factor,
                                (char)(color >> 8) * factor,
                                (char)(color) * factor);
    }
  }
}

void Bar_Matrix::mix_pixel(unsigned char bar, unsigned short pixel, float factor, unsigned char r, unsigned char g, unsigned char b) {
  unsigned int color = bars[bar]->getPixelColor(pixel);
  bars[bar]->setPixelColor(pixel, (char)(color >> 16) * (1-factor) + r * factor,
                                  (char)(color >> 8) * (1-factor) + g * factor,
                                  (char)(color) * (1-factor) + b * factor);
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
