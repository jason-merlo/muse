/*
*  Written by Jason Merlo
*  Last update 9/15/2015
*
*  muse.cpp
*
*  Main program for muse visulizer
*/

// TODO add moving average to bins

/* ======================= includes ================================= */

#include "application.h"
#include "neopixel.h"
#include "math.h"

SYSTEM_MODE(AUTOMATIC);

/* ======================= Defines ================================== */

// Frequency detection
#define NUM_BINS  7
#define LEFT_NOISE_THRESHOLD  118
#define RIGHT_NOISE_THRESHOLD 305
#define HISTORESIS_FACTOR 1.0f/2.0f
#define BINS_MAX          4096.0f

// LED bars
#define LED_TYPE          WS2812B
#define STRIP_LENGTH      70
#define NUM_STRIPS_RIGHT  4
#define NUM_STRIPS_LEFT   4
#define BINS_TO_LEDS      (70.0f/(BINS_MAX - 240))

struct audio_bins {
  // Audio bins
  // [63Hz][160Hz][400Hz][1kHz][2.5kHz][6.25kHz][16kHz][160kHz]
  int left[NUM_BINS];
  int right[NUM_BINS];
};

/*struct led_bars {
// led bars
Adafruit_NeoPixel left[NUM_STRIPS_LEFT];
Adafruit_NeoPixel right[NUM_STRIPS_RIGHT];
};*/

/* ======================= prototypes =============================== */

void init_eq();
void sample_freq(audio_bins* bins);
uint32_t Wheel(byte WheelPos);

/* ======================= muse.cpp ================================= */

// Digital Outputs
const char strobe = D0;
const char rst = D1;
const char led_dat = D2;
const char ps_on = D3;

// Digital Inputs
const char pwr_sw = D4;

// Analog Inputs (0-4096)
const char audio_r = A0;
const char audio_l = A1;

// Declare memory for audio bin structure
struct audio_bins bins;

// Visualizer variables
float color_shift_counter = 0;

Adafruit_NeoPixel bar0 = Adafruit_NeoPixel(STRIP_LENGTH, led_dat, LED_TYPE);

/*
*  Description: Main function - initialize program and loop
*/
void setup() {
  // Set pin modes
  pinMode(rst, OUTPUT);
  pinMode(strobe, OUTPUT);
  pinMode(ps_on, OUTPUT);

  pinMode(pwr_sw, INPUT);
  pinMode(audio_l, INPUT);
  pinMode(audio_r, INPUT);

  // Initialize led bar
  bar0.begin();
  bar0.show();
}

// Appease Arduino :p
void loop() {
  if (color_shift_counter >= 255)
    color_shift_counter = 0;

  color_shift_counter += 0.125;

  // Sample frequency bins
  sample_freq(&bins);

  // Show LED bar
  for (int i = 0; i < bar0.numPixels(); i++) {
    if (i < (float)(bins.right[1]-RIGHT_NOISE_THRESHOLD) * (BINS_TO_LEDS/2))
      bar0.setPixelColor(i, bins.right[0] / 16, bins.right[1] / 16, bins.right[2] / 16);
    else if (i > (float)(BINS_MAX * BINS_TO_LEDS) - ((float)(bins.left[1]-LEFT_NOISE_THRESHOLD) * (BINS_TO_LEDS/2) + 4))
      bar0.setPixelColor(i, bins.left[0] / 16, bins.left[1] / 16, bins.left[2] / 16);
    else
      bar0.setPixelColor(i, Wheel((int)color_shift_counter));
  }
  bar0.show();
};

/*
*  Description: Sends reset sequence to MSGEQ7's allowing for data reading to
*  begin
*/
void init_eq() {
  /*  Reset sequence:
  *
  *  rst     _|‾‾‾‾‾|__
  *
  *  strobe  _|‾‾|__|‾‾
  */
  digitalWrite(rst, HIGH);
  digitalWrite(strobe, HIGH);
  delay(1);
  digitalWrite(strobe, LOW);
  delay(1);
  digitalWrite(rst, LOW);
  digitalWrite(strobe, HIGH);
  delay(1);
}

/*
*  Description: Reads bins from MSGEQ7's and stores them to struct
*  Parameters:  [audio_bins]* bins - frequency bins read from chip
*/
void sample_freq(audio_bins* bins) {
  for (int i = 0; i < NUM_BINS; i++) {
    digitalWrite(strobe, LOW);
    delayMicroseconds(40); // allow for EQ mux to fully switch

    bins->left[i]  = analogRead(audio_l);
    bins->right[i] = analogRead(audio_r);

    digitalWrite(strobe, HIGH);
    delay(1); // allow for EQ mux to fully switch
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return bar0.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return bar0.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return bar0.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
