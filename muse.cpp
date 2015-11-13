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
#include "muse.h"
#include "bar_matrix.h"
#include "application.h"
#include "neopixel.h"

SYSTEM_MODE(AUTOMATIC);

/* ======================= Pin Decls. =============================== */

// Digital Outputs
//static const char ps_on   = A2;
static const char rst     = A3;
static const char strobe  = A4;

// RGB LED Strip Control Shift Register
//static const char sr_lat  = A5;
//static const char sr_clk  = A6;
//static const char sr_dat  = A7;

// Digital Inputs
//static const char pwr_sw = D4;

// Analog Inputs (0-4096)
static const char audio_r = A0;
static const char audio_l = A1;

// Declare memory for audio bin structure
static struct audio_bins bins;

// Declare matrix pins
static const char matrix_pins[8] = {D0, D1, D2, D3, D4, D5, D6, D7};

// Declare matrix variables
static Bar_Matrix* matrix;

//Adafruit_NeoPixel* bar;
/*
 *  setup - initialize program and loop
 */
void setup() {
  // Open debug terminal
  Serial.begin(57600);

  // Set outpus
  //pinMode(ps_on, OUTPUT);
  pinMode(rst, OUTPUT);
  pinMode(strobe, OUTPUT);
  //pinMode(sr_lat, OUTPUT);
  //pinMode(sr_clk, OUTPUT);
  //pinMode(sr_dat, OUTPUT);

  // Set inputs
  //pinMode(pwr_sw, INPUT);
  pinMode(audio_l, INPUT);
  pinMode(audio_r, INPUT);

  // Create new bar matrix inistance
  matrix = new Bar_Matrix(NUM_BARS, STRIP_LENGTH, VERTICAL, LED_TYPE, matrix_pins);

  //bar = new Adafruit_NeoPixel(STRIP_LENGTH, matrix_pins[3], LED_TYPE);
  //bar->begin();
  //bar->show();
}

/*
 *  loop
 */
void loop() {
  // Sample frequency bins
  sample_freq(&bins);

  Serial.println("------right-----");
  // Print to serial terminal
  for (int i = 0; i < NUM_BINS; i++) {
    for (int j = 0; j < (int)(bins.right[i]/40); j++)
      Serial.print("=");
    Serial.println();
  }

  matrix->visualizer_bars(&bins);
  // Keeps the core from being unresponsive to OTA update
  delay(10);
}

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

    bins->left[i]  = analogRead(audio_l) * 0.8 + bins->left[i]  * 0.2;
    bins->right[i] = analogRead(audio_r) * 0.8 + bins->right[i] * 0.2;

    digitalWrite(strobe, HIGH);
    delay(1); // allow for EQ mux to fully switch
  }
}
