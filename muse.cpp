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

 SYSTEM_MODE(AUTOMATIC);

 /* ======================= Defines ================================== */

// Frequency detection
#define NUM_BINS  7

// LED bars
#define LED_TYPE          WS2812B
#define STRIP_LENGTH      70
#define NUM_STRIPS_RIGHT  4
#define NUM_STRIPS_LEFT   4
#define BINS_TO_LEDS      (70.0f/4096.0f)

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

    // Declare memory for led bars structure
    //struct led_bars bars;

    // Initialize LEDs
    //for (int i = 0; i < NUM_STRIPS_RIGHT; i++) {
    //  bars.right[i] = Adafruit_NeoPixel(STRIP_LENGTH, led_dat, LED_TYPE);
    //}

    Adafruit_NeoPixel bar0 = Adafruit_NeoPixel(STRIP_LENGTH, led_dat, LED_TYPE);
    bar0.begin();
    bar0.show();

    int bins_avg = 0;

    // Main loop
    while (1) {
        delay(25);
        Serial.println("------right-----");

        // Check for PS_OK signal, turn on LED
        //digitalWrite(pwr_led, digitalRead(pwr_ok));

        // Sample frequency bins
        sample_freq(&bins);

        // Print to serial terminal
        for (int i = 0; i < NUM_BINS; i++) {
          for (int j = 0; j < (int)(bins.right[i]/51.2); j++)
            Serial.print("=");
          Serial.println();
        }

        bins_avg = 0;

        for (int i = 0; i < NUM_BINS; i++)
          bins_avg += bins.left[i];

        bins_avg /= NUM_BINS;

        // Show LED bar
        for (int i = 0; i < bar0.numPixels(); i++) {
          if (i < (float)bins.right[1] * BINS_TO_LEDS)
            bar0.setPixelColor(i, bins.right[4] / 16, bins.right[5] / 16, bins.right[6] / 16);
          else
            bar0.setPixelColor(i, 115, 0, 115);
        }
        bar0.show();
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
