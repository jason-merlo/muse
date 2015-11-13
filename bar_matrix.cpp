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

/*
 * init_matrix
 *
 * Initializes the matrix to off
 */
void Bar_Matrix::init_matrix() {
  for (int i = 0; i < disp_width; i++) {
    // switch height and width based on orientation
    bars[i]->begin();
    bars[i]->show();
  }
}

/*
 * clear_matrix
 *
 * Blanks matrix
 */
void Bar_Matrix::clear_matrix() {
  for (int i = 0; i < disp_width; i++) {
    for (int j =0; j< disp_height; j++) {
      // switch height and width based on orientation
      bars[i]->setPixelColor(j, 0);
    }
    bars[i]->show();
  }
}

void Bar_Matrix::purple_matrix() {
  for (int i = 0; i < disp_width; i++) {
    for (int j =0; j< disp_height; j++) {
      // switch height and width based on orientation
      bars[i]->setPixelColor(j, Adafruit_NeoPixel::Color(255,0,255));
    }
    bars[i]->show();
  }
}

void Bar_Matrix::visualizer_wheel(float WheelPos) {
  for (char j = 0; j < disp_width; j++) {
    for (char i = 0; i < disp_height; i++) {
      if(WheelPos < 85) {
        bars[j]->setPixelColor(i, Adafruit_NeoPixel::Color((WheelPos * 3), (255 - WheelPos * 3), 0));
      } else if(WheelPos < 170) {
        bars[j]->setPixelColor(i, Adafruit_NeoPixel::Color((255 - (WheelPos-85) * 3), 0, ((WheelPos-85) * 3)));
      } else {
        bars[j]->setPixelColor(i, Adafruit_NeoPixel::Color(0, ((WheelPos-170) * 3), (255 - (WheelPos-170) * 3)));
      }
    }
    bars[j]->show();
  }
}

/* WARNING: Non-scalable code below */
/* TODO fix Non-scalable code */
void Bar_Matrix::visualizer_bars(audio_bins* bins) {
  for (char i = 0; i < disp_width; i++) {
    for (char j = 0; j < disp_height; j++) {
      // get bin
      int level = (i < disp_width/2) ? bins->left[i] : bins->left[disp_width - i];
      // set bar
      if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH))
        bars[i]->setPixelColor(j, 0, 255, 0);
      else
        bars[i]->setPixelColor(j, 255, 0, 255);
    }
    // Show bar
    bars[i]->show();
  }
}
