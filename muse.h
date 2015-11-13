#ifndef MUSE_H
#define MUSE_H

#include "application.h"
#include "neopixel.h"

/* ======================= Defines ================================== */

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

struct audio_bins {
  // Audio bins
  // [63Hz][160Hz][400Hz][1kHz][2.5kHz][6.25kHz][16kHz][160kHz]
  int left[NUM_BINS];
  int right[NUM_BINS];
};

/* ======================= prototypes =============================== */

void init_eq();
void sample_freq(audio_bins* bins);
uint32_t Wheel(byte WheelPos, float intensity);
uint32_t Color(uint8_t r, uint8_t g, uint8_t b);


#endif // MUSE_H
