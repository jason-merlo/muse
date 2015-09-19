/*
 *  Written by Jason Merlo
 *  Last update 9/15/2015
 *
 *  muse.cpp
 *
 *  Main program for muse visulizer
 */

 /* ======================= includes ================================= */

 #include "application.h"
 #include "neopixel.h"

 SYSTEM_MODE(AUTOMATIC);

 /* ======================= Defines ================================== */

#define NUM_BINS  7

struct audio_bins {
    // Audio bins
    int left[NUM_BINS];
    int right[NUM_BINS];
};

 /* ======================= prototypes =============================== */

void init_eq();
void sample_freq(audio_bins* bins);

 /* ======================= muse.cpp ================================= */

 // Digital Outputs
const char rst = D0;
const char strobe = D1;
const char led_dat = D2;
const char ps_on = D3;
const char pwr_led = D4;

// Digital Inputs
const char pwr_ok = D5;
const char pwr_sw = D6;

// Analog Inputs (0-4096)
const char audio_r = A0;
const char audio_l = A1;

/*
 *  Description: Main function - initialize program and loop
 */
void setup() {
    // Open debug terminal
    Serial.begin(57600);

    // Set pin modes
    pinMode(rst, OUTPUT);
    pinMode(strobe, OUTPUT);
    pinMode(ps_on, OUTPUT);
    pinMode(pwr_led, OUTPUT);

    pinMode(pwr_ok, INPUT);
    pinMode(pwr_sw, INPUT);
    pinMode(audio_l, INPUT);
    pinMode(audio_r, INPUT);

    // Declare memory for audio bin structure
    struct audio_bins bins;

    while (1) {
        delay(1);
        Serial.println("-------------");

        // Sample frequency bins
        sample_freq(&bins);

        // Print to serial terminal
        for (int i = 0; i < NUM_BINS; i++) {
          Serial.print(bins.right[i]);
          Serial.print("\t");
          Serial.println(bins.left[i]);
        }
    }
}

// Appease Arduino :p
void loop();

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

        bins->left[i] = analogRead(audio_l);
        bins->right[i] = analogRead(audio_r);

        digitalWrite(strobe, HIGH);
        delay(1); // allow for EQ mux to fully switch
    }
}
