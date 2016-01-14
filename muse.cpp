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
static const char ps_on   = TX;
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
static int bar_levels[4];

// Declare matrix pins
static const char matrix_pins[8] = {D0, D1, D2, D3, D4, D5, D6, D7};

// Declare matrix variables
static Bar_Matrix* matrix;

/* =============== Visualizer variables ================= */

#if ENABLE_PSU_CONTROL
static bool psu_is_on = false;
#endif

#if RUN_BOUNCING_BARS || ENABLE_SCREENSAVER
static unsigned long bouncing_lines_last_update;
#endif

#if ENABLE_SCREENSAVER
static unsigned long last_sound_seconds;
#endif

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
  //pinMode(pwr_sw, INPUT);
  psu_is_on = false;
  psu_startup();
  #endif

  // Enables pins for shift register to control RGB LED strips
  #if ENABLE_RGB_SR
  pinMode(sr_lat, OUTPUT);
  pinMode(sr_clk, OUTPUT);
  pinMode(sr_dat, OUTPUT);
  #endif

  // Create new bar matrix inistance
  #if ENABLE_BARS
  matrix = new Bar_Matrix(NUM_BARS, STRIP_LENGTH, LED_TYPE, matrix_pins);
  #endif

  // Initialize screenaver variables
  #if ENABLE_SCREENSAVER
  last_sound_seconds = Time.now();
  #endif

  // Initialize bouncing bars variables
  #if RUN_BOUNCING_BARS || ENABLE_SCREENSAVER
  bouncing_lines_last_update = 0;
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

  #if ENABLE_SCREENSAVER
  // Check each bin to see if they are below the threshold to be "off"
  // If any bin is active break and just run the visualizer
  bool any_bin_active = false;
  for (int i = 0; i < NUM_BINS; i++) {
    if (bins.right[i] > SCREENSAVER_MINIMUM || bins.left[i] > SCREENSAVER_MINIMUM) {
      any_bin_active = true;
      if (!psu_is_on) { psu_startup(); }
      last_sound_seconds = Time.now();
      break;
    }
  }

  if (any_bin_active || Time.now()-last_sound_seconds < SCREENSAVER_SECS_TO_START) {
    // Run the visualizer if any bin is active. Insert your favorite visualizer here
    matrix->visualizer_bars_middle(&bins, 0.15, 0.8, bar_levels);
    matrix->show_all();

  } else if (Time.now()-last_sound_seconds > SCREENSAVER_SECS_TO_PSU_OFF) {
    // If we have passed the seconds until psu shutoff, turn it off
    if (psu_is_on) { psu_shutdown(); }

  } else {
    // Otherwise we must be in the screensaver time. Run the screensaver
    if (psu_is_on && millis() - bouncing_lines_last_update > 10) {
      matrix->bouncing_lines();
      matrix->show_all();
      bouncing_lines_last_update = millis();
    }
  }
  #endif

  #if RUN_BOUNCING_BARS
  if (millis() - bouncing_lines_last_update > 10) {
    matrix->bouncing_lines();
    matrix->show_all();
    bouncing_lines_last_update = millis();
  }
  #endif

  #if RUN_COLOR_WHEEL
  matrix->visualizer_wheel(0.25, 10);
  #endif

  #if RUN_VISUALIZER_BARS
  matrix->visualizer_bars(&bins, 0.15, 0.8, bar_levels);
  matrix->show_all();
  #endif

  #if RUN_VISUALIZER_BARS_MIDDLE
  matrix->visualizer_bars_middle(&bins, 0.15, 0.8, bar_levels);
  matrix->show_all();
  #endif
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
 *  Function: psu_shutdown
 *  Description: turns the psu off, sets psu_is_on to false
 *  Parameters:  none
 * ================================================================== */
void psu_shutdown() {
 if (psu_is_on) {
   matrix->clear_matrix();
   matrix->show_all();
   digitalWrite(ps_on, HIGH);
 }
 psu_is_on = false;
}

 /* ================================================================== *
  *  Function: psu_startup
  *  Description: Turns the psu on
  *  Parameters:  none
  * ================================================================== */
void psu_startup() {
  if (!psu_is_on) {
    matrix->clear_matrix();
    matrix->show_all();
    digitalWrite(ps_on, LOW);
  }
  psu_is_on = true;
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
    delayMicroseconds(40); // allow for EQ mux to fully switch
  }
}
