/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  updates:
 *  11/17/2015
 *
 *  File: muse.h
 *
 *  Description: Header for muse visualizer containing function flags,
 *  program constants, structs, and prototypes
 * ================================================================== */

#ifndef MUSE_H
#define MUSE_H

#include "application.h"
#include "neopixel.h"

/* ======================= Define - Flags =========================== */

#define ENABLE_BARS           1
#define ENABLE_MSGEQ7         1
#define ENABLE_PSU_CONTROL    1
#define ENABLE_RGB_SR         0
#define ENABLE_SCREENSAVER    0
#define ENABLE_AUTO_SHUTDOWN  1
#define ENABLE_WEB_SERVER     1
#define ENABLE_SERIAL         0

/* ======================= Define - General ========================= */

// Frequency detection
#define NUM_BINS  7
#define LEFT_NOISE_THRESHOLD  0 //118
#define RIGHT_NOISE_THRESHOLD 0 //308
#define HISTORESIS_FACTOR 1.0f/2.0f
#define BINS_MAX          4096.0f

// LED bars
#define LED_TYPE          WS2812B
#define STRIP_LENGTH      70
#define NUM_BARS          8
#define BINS_TO_LEDS      (70.0f/(BINS_MAX - 240))
#define FREQ_GAIN         1.0f

// Screensaver constants
#define SCREENSAVER_MINIMUM         1300
#define SCREENSAVER_SECS_TO_PSU_OFF 30
#define SCREENSAVER_SECS_TO_START   10

/* ======================= Define - Visualziers ===================== */
#define VISUALIZER_WHEEL          0
#define VISUALIZER_BARS           1
#define VISUALIZER_BARS_MIDDLE    2
#define VISUALIZER_PULSE          3
#define VISUALIZER_PLASMA         4
#define VISUALIZER_RAINBOW        5

#define BOUNCING_LINES            77

#define BAR_TEST                  88
#define PIXEL_TEST                99

/* ======================= Define - EQ Bins ===================== */
#define LEFT_63                   0 //varified
#define LEFT_160                  1
#define LEFT_400                  2 //verified
#define LEFT_1000                 3
#define LEFT_2500                 4 //verified
#define LEFT_6250                 5 //verified
#define LEFT_16000                6

#define RIGHT_63                  0 //verified
#define RIGHT_160                 1
#define RIGHT_400                 2 //verified
#define RIGHT_1000                3
#define RIGHT_2500                4 //verified
#define RIGHT_6250                5 //verified
#define RIGHT_16000               6

/* ======================= Structs - General ======================== */

// Struct to hold MSGEQ7 frequency intensity bins
struct audio_bins {
    // Audio bins
    // [63Hz][160Hz][400Hz][1kHz][2.5kHz][6.25kHz][16kHz][160kHz]
    int left[NUM_BINS];
    int right[NUM_BINS];
};

// Struct to hold color values
struct Color_Value {
    unsigned int c;
    unsigned char r, g, b;

    // TODO: add code to convert from HSV to RGB and vice versa in
    //       constructor.
    Color_Value(unsigned char red, unsigned char green, unsigned char blue) :
    c((red << 16) | (green << 8) | blue), r(red), g(green), b(blue) {}
};

/* ======================= prototypes =============================== */

void init_eq();
void psu_shutdown();
void psu_startup();
void sample_freq(audio_bins* bins);
void serve_webpage();
uint32_t Wheel(byte WheelPos, float intensity);
uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

#endif // MUSE_H
