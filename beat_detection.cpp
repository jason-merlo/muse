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
    beat_reporter = false;
    flip_on = false;

    red = 0;
    green = 0;
    blue = 0;
    beat_count = 0;
    beats_per_flip = 3;
    sma_long_index = 0;
    sma_short_index = 0;

    bpm_long_total = 0.0;
    bpm_short_total = 0.0;

    for (int i = 0; i < NUM_BINS; i++) {
        sma_long_totals[i] = 0.0;
        sma_short_totals[i] = 0.0;
        beat_on_bins[i] = false;
        beat_reporter_bins[i] = false;
    }
}

/* ================================================================== *
 * Function: tick
 * Description: The main update function for all beat detection.
 * Parameters: [audio_bins *] bins - The visualizer's audio_bins object
 * ================================================================== */
void Beat_Detection::tick(audio_bins* bins) {
    tick_bpm_detection(bins);
    for (int i = 0; i < NUM_BINS; i++) {
        tick_beat_detection(bins, i);
    }

    sma_long_index++;
    sma_long_index = sma_long_index % SMA_LONG_LENGTH;
    sma_short_index++;
    sma_short_index = sma_short_index % SMA_SHORT_LENGTH;
}

void Beat_Detection::tick_beat_detection(audio_bins * bins, int cur_bin) {
    sma_short_totals[cur_bin] -= sma_short_bins[cur_bin][sma_short_index];
    sma_short_bins[cur_bin][sma_short_index]  = (bins->left[cur_bin]*bins->left[cur_bin])/2;
    sma_short_bins[cur_bin][sma_short_index] += (bins->right[cur_bin]*bins->right[cur_bin])/2;
    sma_short_totals[cur_bin] += sma_short_bins[cur_bin][sma_short_index];
    float sma_short = sma_short_totals[cur_bin] / SMA_SHORT_LENGTH;

    sma_long_totals[cur_bin] -= sma_long_bins[cur_bin][sma_long_index];
    sma_long_bins[cur_bin][sma_long_index] = sma_short_bins[cur_bin][sma_short_index];
    sma_long_totals[cur_bin] += sma_long_bins[cur_bin][sma_long_index];
    float sma_long = sma_long_totals[cur_bin] / SMA_LONG_LENGTH;

    if (!beat_on_bins[cur_bin] && sma_short > 1.6*sma_long) {
        //beat detected
        // beat_reporter is used by visualizers, it is cleared after every frame
        // beat_on is used by beat detection, it is cleared whenever a beat ends
        beat_on_bins[cur_bin] = true;
        beat_reporter_bins[cur_bin] = true;
    } else if (beat_on_bins[cur_bin] && sma_short < 1.0*sma_long) {
        //beat reset
        beat_on_bins[cur_bin] = false;
    }
}

void Beat_Detection::tick_bpm_detection(audio_bins * bins) {
    bpm_short_total -= bpm_short_values[sma_short_index];
    bpm_short_values[sma_short_index] = (bins->left[0]*bins->left[0] + bins->right[0]*bins->right[0])/4;
    bpm_short_values[sma_short_index] += (bins->left[1]*bins->left[1] + bins->right[1]*bins->right[1])/4;
    //bpm_short_values[sma_short_index] = (bins->left[0]*bins->left[0] + bins->left[1]*bins->left[1])/2;
    bpm_short_total += bpm_short_values[sma_short_index];
    float sma_short = bpm_short_total / SMA_SHORT_LENGTH;

    bpm_long_total -= bpm_long_values[sma_long_index];
    bpm_long_values[sma_long_index] = bpm_short_values[sma_short_index];
    bpm_long_total += bpm_long_values[sma_long_index];
    float sma_long = bpm_long_total / SMA_LONG_LENGTH;

    if (!beat_on && sma_short > 1.40*sma_long) {
        //beat detected
        beat_count++;

        if (beat_count % beats_per_flip == 0) {
            flip_on = !flip_on;
        }

        blue = (green+red) % 255;
        green = red;
        red = (red+random(255)) % 255;

        // beat_reporter is used by visualizers, it is cleared after every frame
        // beat_on is used by beat detection, it is cleared whenever a beat ends
        beat_on = true;
        beat_reporter = true;
    } else if (beat_on && sma_short < 1.20*sma_long) {
        //beat reset
        beat_on = false;
    }
}

/* ================================================================== *
 * Function: frame_ticked
 * Description: Clear all beat detection reporters
 * Parameters: none
 * ================================================================== */
void Beat_Detection::frame_ticked() {
    beat_reporter = false;
    for (int i = 0; i < NUM_BINS; i++) {
        beat_reporter_bins[i] = false;
    }
}

/* ================================================================== *
 * Function: beat_on_bin
 * Description: Returns the beat reporter for the given bin
 * Parameters: [int] bin - The bin to check
 * Returns: True if a beat was detected within the last frame, false otherwise.
 * ================================================================== */
bool Beat_Detection::beat_on_bin(int bin) {
    return beat_reporter_bins[bin];
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
