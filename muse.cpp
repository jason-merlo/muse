/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  updates:
 *  11/17/2015
 *
 *  File: muse.cpp
 *
 *  Description: Main program for muse visulizer
 * ================================================================== */

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
static const char sr_lat  = A5;
static const char sr_clk  = A6;
static const char sr_dat  = A7;

// Digital Inputs
static const char pwr_sw = D4;

// Analog Inputs (0-4096)
static const char audio_r = A0;
static const char audio_l = A1;

// Declare memory for audio bin structure
static struct audio_bins bins;

// Declare matrix pins
static const char matrix_pins[8] = {D0, D1, D2, D3, D4, D5, D6, D7};

// Declare matrix variables
static Bar_Matrix* matrix;

/* ================================================================== *
 *  Function: Setup
 *  Description: initialize program I/O and data structures
 * ================================================================== */
void setup() {

  // Open debug terminal
  #if ENABLE_SERIAL
  Serial.begin(57600);
  #endif

  // Enables frequency analyzer
  #if ENABLE_MSGEQ7
  pinMode(rst, OUTPUT);
  pinMode(strobe, OUTPUT);
  pinMode(audio_l, INPUT);
  pinMode(audio_r, INPUT);
  #endif

  // Enables power switch input for PSU and control for
  #if ENABLE_PSU_CONTROL
  pinMode(ps_on, OUTPUT);
  pinMode(pwr_sw, INPUT);
  #endif

  // Enables pins for shift register to control RGB LED strips
  #if ENABLE_RGB_SR
  pinMode(sr_lat, OUTPUT);
  pinMode(sr_clk, OUTPUT);
  pinMode(sr_dat, OUTPUT);
  #endif

  // Create new bar matrix inistance
  #if ENABLE_BARS
  matrix = new Bar_Matrix(NUM_BARS, STRIP_LENGTH, VERTICAL, LED_TYPE, matrix_pins);
  #endif
}

/* ================================================================== *
 *  Function: loop
 *  Description: Contains main program
 * ================================================================== */
void loop() {
  // Sample frequency bins
  #if ENABLE_MSGEQ7
  sample_freq(&bins);
  #endif

  #if ENABLE_BARS
  matrix->decay(0.75);
  matrix->visualizer_bars(&bins);
  matrix->show_all();
  #endif

  // DO NOT REMOVE: Keeps the core from being unresponsive to OTA
  // update, requiring hard reset
  //delay(1.15);
}

/* ================================================================== *
 *  Function: init_eq
 *  Description: Sends reset sequence to MSGEQ7's allowing for data
 *  reading to begin
 * ================================================================== */
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

/* ================================================================== *
 *  Function: sample_freq
 *  Description: Reads bins from MSGEQ7's and stores them to struct
 *  Parameters:  [audio_bins]* bins - frequency bins read from chip
 * ================================================================== */
void sample_freq(audio_bins* bins) {
  for (int i = 0; i < NUM_BINS; i++) {
    digitalWrite(strobe, LOW);
    delayMicroseconds(40); // allow for EQ mux to fully switch

    // Moving average bins with new sample
    bins->left[i]  = analogRead(audio_l) * 0.8 + bins->left[i]  * 0.2;
    bins->right[i] = analogRead(audio_r) * 0.8 + bins->right[i] * 0.2;

    digitalWrite(strobe, HIGH);
    delay(1); // allow for EQ mux to fully switch
  }
}
