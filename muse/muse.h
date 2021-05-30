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

// Include defines specific to each hardware instance/user
#include "settings.h"

/* ======================= Define - General ========================= */

// Frequency detection
#define NUM_BINS  7
#define LEFT_NOISE_THRESHOLD  0 //118
#define RIGHT_NOISE_THRESHOLD 0 //308
#define HISTORESIS_FACTOR 1.0f/2.0f
#define BINS_MAX          4095.0f

// LED bars
#define LED_TYPE          WS2812B
#define STRIP_LENGTH      58
#define NUM_BARS          10
#define BINS_TO_LEDS      (58.0f/(BINS_MAX - 240))
#define FREQ_GAIN         1.0f

// Update intervals, min time between updates of subsystems in millis
#define DISPLAY_UPDATE_INTERVAL     15
#define MDNS_UPDATE_INTERVAL        48
#define SERVER_UPDATE_INTERVAL      48
#define SAMPLE_UPDATE_INTERVAL      10

/* ======================= Define - Visualziers ===================== */
#define VISUALIZER_BARS         0
#define VISUALIZER_BARS_MIDDLE  1
#define VISUALIZER_PLASMA       2
#define VISUALIZER_PULSE        3
#define VISUALIZER_RAINBOW      4
#define VISUALIZER_WHEEL        5
#define VISUALIZER_CLASSIC      6
#define VISUALIZER_PONG         7
#define VISUALIZER_BASS_MIDDLE  8
#define VISUALIZER_BASS_SLIDE   9

#define AMBIENT_LIGHTING        66

#define BOUNCING_LINES          77
#define SNAKE_LINES             78

#define BAR_TEST                88
#define PIXEL_TEST              99

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
void powered_on_tick();
void psu_shutdown();
void psu_startup();
void sample_freq(audio_bins* bins);
uint32_t Wheel(byte WheelPos, float intensity);
uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

#endif // MUSE_H
