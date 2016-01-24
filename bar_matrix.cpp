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
  for(short i = 0; i < num_bars; i++) {
    bars[i] = new Adafruit_NeoPixel(bar_len, pins[i], led_type);

    // Set up variables to track bouncing lines
    bouncing_line_lengths[i] = random(0, STRIP_LENGTH / 2);
    bouncing_line_positions[i] = random(0, STRIP_LENGTH);
    //bouncing_line_positions[i] = (i%2 == 0) ? 0 : STRIP_LENGTH;
    if (random(1) > .5) bouncing_line_directions[i] = 1;
    else                bouncing_line_directions[i] = -1;
  }

  init_matrix();
  clear_matrix();
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
  }
}

/* ================================================================== *
 * Function: bouncing_lines
 * Description: Bounces a solid line of LEDs up and down in each matrix bar
 * Parameters: none
 * ================================================================== */
void Bar_Matrix::bouncing_lines() {
  for (int i = 0; i < disp_width; i++) {
    int bottom = bouncing_line_positions[i];

    // Clear the pixel the line just left, light the one it entered
    if (bouncing_line_directions[i] == 1) {
      if (bottom > 0) bars[i]->setPixelColor(bottom-1, 0, 0, 0);
      bars[i]->setPixelColor(1+bottom+bouncing_line_lengths[i], 60, 35, 7);
    } else {
      if (bottom > 0) bars[i]->setPixelColor(bottom-1, 0, 32, 91);
      bars[i]->setPixelColor(1+bottom+bouncing_line_lengths[i], 0, 0, 0);
    }

    // Move the line
    bouncing_line_positions[i] += bouncing_line_directions[i];

    // Ensure we are moving in the proper direction
    if (bouncing_line_positions[i] < -.5*bouncing_line_lengths[i] && bouncing_line_directions[i] < 0) {
      bouncing_line_directions[i] = 1;
    } else if (bouncing_line_positions[i] > STRIP_LENGTH-.5*bouncing_line_lengths[i] && bouncing_line_directions[i] > 0) {
      bouncing_line_directions[i] = -1;
    }
  }
}

/* ================================================================== *
 * Function: visualizer_wheel
 * Description: Sets all bars to a color while rotating through all
 *              hues in order of the rainbow
 * Parameters: [float] hue - hue value, or rotation of wheel
 *             [float] intensity - intensity of lights
 * ================================================================== */
void Bar_Matrix::visualizer_wheel(float intensity, float speed) {
  // TODO: incorporate internal hue "rotation" algorithm
  float val = fmod(millis()/10000.0f,1.0f)*2.0f*PI;
  fill_matrix(Color_Value(cos(val)*255*intensity, cos(val - 2*PI/3)*255*intensity, cos(val - 4*PI/3)*255*intensity));
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
      level *= FREQ_GAIN;
      // set bar
      if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH)) {
      //if (j < (float)(level)/(float)(BINS_MAX) * (STRIP_LENGTH)) {

        float val = level*2*PI/4096.0;
        mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
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
 * Function: visualizer_bars_middle
 * Description: Bars start at the middle and go to the edges.
 *              One channel fills up, the other fills down.
 * Parameters: none.
 * ================================================================== */
void Bar_Matrix::visualizer_bars_middle(audio_bins* bins, float in_factor, float out_factor, int* bar_levels) {
  decay(out_factor);

  // Left bins, grows downwards
  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < STRIP_LENGTH/2; j++) {
      // get bin
      int level = bins->left[i%(NUM_BARS-1)];
      level *= FREQ_GAIN;
      // set bar
      if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH/2)) {
        float val = level*2*PI/4096.0;
        mix_pixel(i, STRIP_LENGTH/2 - j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
      }
    }

    // Right bins, grow upwards
    for (char j = STRIP_LENGTH/2; j < STRIP_LENGTH; j++) {
      // get bin
      int level = bins->right[i%(NUM_BARS-1)];
      level *= FREQ_GAIN;
      // set bar
      if (j-STRIP_LENGTH/2 < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH/2)) {
        float val = level*2*PI/4096.0;
        mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
      }
    }
  }
}

void Bar_Matrix::visualizer_rainbow(audio_bins* bins, float in_factor, float out_factor, int* bar_levels) {
  decay(out_factor);

  for (char i = 0; i < disp_width/2; i++) {
    int led_index = 0;
    for (char j = 0; j < STRIP_LENGTH; j+=10) {
      // get bin
      int level = bins->left[j/10];
      level *= FREQ_GAIN;
      // set bar
      if (i < (pow((float)(level)/(float)(BINS_MAX), 2)) * (disp_width/2)) {
        float val = level*2*PI/4096.0;
        for (int x = 0; x < 10; x++) {
          mix_pixel((disp_width/2)-i-1, x*NUM_BARS+led_index, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
        }
      }

      led_index++;
      led_index = led_index%10;
    }
  }

  for (char i = disp_width/2; i < disp_width; i++) {
    int led_index = 0;
    for (char j = 0; j < STRIP_LENGTH; j+=10) {
      // get bin
      int level = bins->right[j/10];
      level *= FREQ_GAIN;
      // set bar
      if (i-disp_width/2 < (pow((float)(level)/(float)(BINS_MAX), 2)) * (disp_width/2)) {
        float val = level*2*PI/4096.0;
        for (int x = 0; x < 10; x++) {
          mix_pixel(i, x*NUM_BARS+led_index, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
        }
      }

      led_index++;
      led_index = led_index%10;
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
