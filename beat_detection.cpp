/* ================================================================== *
*  Written by Steven Ploog
*
*  updates:
*
*  File: beat_detection.cpp
*
*  Description: Class to handle beat detection functions/data
* ================================================================== */

#include "beat_detection.h"

/* ================================================================== *
 * Function: Beat_Detection
 * Description: Create a beat detection instance
 * Parameters: [audio_bins *] bins - The visualizer's audio_bins object
 * ================================================================== */
Beat_Detection::Beat_Detection() {
    beat_on = false;
    flip_on = false;

    red = 0;
    green = 0;
    blue = 0;
    beat_count = 0;
    beats_per_flip = 3;

    sma_long_total = 0;
    sma_long_index = 0;
    sma_short_total = 0;
    sma_short_index = 0;
}

/* ================================================================== *
 * Function: tick
 * Description: The main update function for all beat detection.
 * Parameters: [audio_bins *] bins - The visualizer's audio_bins object
 * ================================================================== */
void Beat_Detection::tick(audio_bins* bins) {
    sma_short_total -= sma_short_values[sma_short_index];
    sma_short_values[sma_short_index] = (bins->left[0]*bins->left[0] + bins->left[1]*bins->left[1])/2;
    sma_short_total += sma_short_values[sma_short_index];
    float sma_short = sma_short_total / SMA_SHORT_LENGTH;

    sma_long_total -= sma_long_values[sma_long_index];
    sma_long_values[sma_long_index] = sma_short_values[sma_short_index];
    sma_long_total += sma_long_values[sma_long_index];
    float sma_long = sma_long_total / SMA_LONG_LENGTH;

    if (!beat_on && sma_short > 1.00*sma_long) {
        //beat detected
        beat_count++;

        if (beat_count % beats_per_flip == 0) {
            flip_on = !flip_on;
        }

        blue = (green+red) % 255;
        green = red;
        red += random(255);

        beat_on = true;
    } else if (beat_on && sma_short < 1.00*sma_long) {
        //beat reset
        beat_on = false;
    }

    sma_long_index++;
    sma_long_index = sma_long_index % SMA_LONG_LENGTH;
    sma_short_index++;
    sma_short_index = sma_short_index % SMA_SHORT_LENGTH;
}

/* ================================================================== *
 * Function: flip
 * Description: Returns the current value of flip_on
 * Parameters: none
 * ================================================================== */
bool Beat_Detection::flip() {
    return flip_on;
}

/* ================================================================== *
 * Function: num_beats
 * Description: Returns the current number of beats detected
 * Parameters: none
 * ================================================================== */
int Beat_Detection::num_beats() {
    return beat_count;
}

/* ================================================================== *
 * Function: r
 * Description: Returns the current red value
 * Parameters: none
 * ================================================================== */
int Beat_Detection::r() {
    return red;
}

/* ================================================================== *
 * Function: g
 * Description: Returns the current green value
 * Parameters: none
 * ================================================================== */
int Beat_Detection::g() {
    return green;
}

/* ================================================================== *
 * Function: b
 * Description: Returns the current blue value
 * Parameters: none
 * ================================================================== */
int Beat_Detection::b() {
    return blue;
}

/* ================================================================== *
 * Function: set_beats_per_flip
 * Description: Change the numbers of beat detections needed to toggle flip_on
 * Parameters: [int] beats - the number of beats required to toggle flip_on
 * ================================================================== */
void Beat_Detection::set_beats_per_flip(int beats) {
    if (beats > 0)
        beats_per_flip = beats;
}
