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
void Bar_Matrix::bouncing_lines(float speed) {
  for (int i = 0; i < disp_width; i++) {
    int bottom = bouncing_line_positions[i];

    // Clear the pixel the line just left, light the one it entered
    if (bouncing_line_directions[i] == 1) {
      if (bottom > 0) mix_pixel(i, bottom-1, 1, 0, 0, 0);
      mix_pixel(i, 1+bottom+bouncing_line_lengths[i], 1, 64, 64, 64);
    } else {
      if (bottom > 0) mix_pixel(i, bottom-1, 1, 0, 64, 16);
      mix_pixel(i, 1+bottom+bouncing_line_lengths[i], 1, 0, 0, 0);
    }

    // Move the line
    bouncing_line_positions[i] += bouncing_line_directions[i] * speed;

    // Ensure we are moving in the proper direction
    if (bouncing_line_positions[i] < -.5*bouncing_line_lengths[i] && bouncing_line_directions[i] < 0) {
      bouncing_line_directions[i] = 1;
    } else if (bouncing_line_positions[i] > STRIP_LENGTH-.5*bouncing_line_lengths[i] && bouncing_line_directions[i] > 0) {
      bouncing_line_directions[i] = -1;
    }
  }
}

/* ================================================================== *
 * Function: bar_test
 * Description: Turns on bars in order to test wiring
 * Parameters: None
 * ================================================================== */
void Bar_Matrix::bar_test() {
  for (int i = 0; i < STRIP_LENGTH; i++)
    bars[int(millis()/1000)%8]->setPixelColor(i, 64, 64, 64);
}


/* ================================================================== *
 * Function: visualizer_wheel
 * Description: Sets all bars to a color while rotating through all
 *              hues in order of the rainbow
 * Parameters: [float] intensity - intensity of lights
 *             [float] speed - speed wheel rotates at
 * ================================================================== */
void Bar_Matrix::visualizer_wheel(float intensity, float speed) {
  float val = fmod(millis()/10000.0f,1.0f)*2.0f*PI;
  fill_matrix(Color_Value(cos(val)*255*intensity, cos(val - 2*PI/3)*255*intensity, cos(val - 4*PI/3)*255*intensity));
}

/* ================================================================== *
 * Function: visualizer_pulse
 * Description: Creates pulses where sound appears to be originating from
 * Parameters:  None
 * ================================================================== */
void Bar_Matrix::visualizer_pulse(audio_bins* bins, float in_factor, float out_factor, float distance_x, float distance_y) {
  decay (out_factor);

  // Calculate frequency pan and "fade"
  float pans[NUM_BINS]; // ratio left to right, 0-1
  float intensities[NUM_BINS]; // ratio low to high, 0-1
  for (char i = 0; i < NUM_BINS; i++)
    pans[i] = float(bins->left[i])/float(bins->right[i]);
  for (char i = 0; i < NUM_BINS; i++)
    intensities[i] = ((float(bins->left[i])+float(bins->right[i]))/2.0f)/4096.0f;

  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < disp_height; j++) {
      float level = 0;

      // Calculate level intensity
      for (char x = 0; x < NUM_BINS; x++) {
        float distance = distance_x * 1.0f/(abs(pans[x]-(i/disp_width))) + distance_y * 1.0f/(abs((x/NUM_BINS)-(j/disp_height)));
        level += intensities[x] * distance;
      }

      //level *= 2*PI;
      //mix_pixel(i, j, in_factor, cos(level)*255*level, cos(level - 2*PI/3)*255*level, cos(level - 4*PI/3)*255*level);
      mix_pixel(i, j, in_factor, level*255, level*255, level*255);
    }
  }
}

/* ================================================================== *
 * Function: fill_matrix
 * Description: Sets all pixel values to given color value
 * Parameters: [audio_bins]* bins - frequency bins read from chip
 * ================================================================== */
void Bar_Matrix::visualizer_bars(audio_bins* bins, float in_factor, float out_factor, bool strobe) {
  decay(out_factor);
  float bass_level = 0;

  if (strobe)
    bass_level = (log(((bins->left[LEFT_63]+bins->right[LEFT_63])/2.0f)/4096.0f)+0.7f) * 5 * 255.0f;

  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < disp_height; j++) {
      // get bin
      //int level = (i < disp_width/2) ? bar_levels[i] : bar_levels[7-i];

      if (strobe)
        mix_pixel(i, j, 0.5f, bass_level, bass_level, bass_level);

      // Set bar levels
      int level = 0;
      switch(i) {
        case 0:
          level = bins->left[LEFT_160]; //1kHz
          break;
        case 1:
          level = bins->left[LEFT_1000]; //6.25kHz
          break;
        case 2:
          level = bins->left[LEFT_6250]; //63Hz
          break;
        case 3:
          level = bins->left[LEFT_16000]; //400Hz
          break;
        case 4:
          level = bins->right[RIGHT_16000]; //400Hz
          break;
        case 5:
          level = bins->right[RIGHT_6250]; //63Hz
          break;
        case 6:
          level = bins->right[RIGHT_1000]; //6.25kHz
          break;
        case 7:
          level = bins->right[RIGHT_160]; //1kHz
          break;
      }

      level *= FREQ_GAIN;
      // set bar
      if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH)) {
      //if (j < (float)(level)/(float)(BINS_MAX) * (STRIP_LENGTH))

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
void Bar_Matrix::visualizer_bars_middle(audio_bins* bins, float in_factor, float out_factor) {
  decay(out_factor);

  // Right bins, grows downwards
  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < STRIP_LENGTH/2; j++) {
      // get bin
      // Set bar levels
      int level = 0;
      switch(i) {
        case 0:
          level = bins->right[RIGHT_63];
          break;
        case 1:
          level = bins->right[RIGHT_160];
          break;
        case 2:
          level = bins->right[RIGHT_400];
          break;
        case 3:
          level = bins->right[RIGHT_1000];
          break;
        case 4:
          level = bins->right[RIGHT_2500];
          break;
        case 5:
          level = bins->right[RIGHT_6250];
          break;
        case 6:
          level = (bins->right[RIGHT_16000] + bins->right[RIGHT_6250])/2;
          break;
        case 7:
          level = bins->right[RIGHT_16000];
          break;
      }
      level *= FREQ_GAIN;
      // set bar
      if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH/2)) {
        float val = level*2*PI/4096.0;
        mix_pixel(i, STRIP_LENGTH/2 - j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
      }
    }

    // Left bins, grow upwards
    for (char j = STRIP_LENGTH/2; j < STRIP_LENGTH; j++) {
      // get bin
      int level = 0;
      switch(i) {
        case 0:
          level = bins->left[LEFT_63];
          break;
        case 1:
          level = bins->left[LEFT_160];
          break;
        case 2:
          level = bins->left[LEFT_400];
          break;
        case 3:
          level = bins->left[LEFT_1000];
          break;
        case 4:
          level = bins->left[LEFT_2500];
          break;
        case 5:
          level = bins->left[LEFT_6250];
          break;
        case 6:
          level = (bins->left[LEFT_16000] + bins->left[LEFT_6250])/2;
          break;
        case 7:
          level = bins->left[LEFT_16000];
          break;
      }
      level *= FREQ_GAIN;
      // set bar
      if (j-STRIP_LENGTH/2 < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH/2)) {
        float val = level*2*PI/4096.0;
        mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*PI/3)*255, cos(val - 4*PI/3)*255);
      }
    }
  }
}

/* ================================================================== *
 * Function: decay
 * Description: slowly fades out matrix values
 * Parameters: [float] factor - decay factor to be multiplied by
 * ================================================================== */
void Bar_Matrix::decay(double factor) {
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
