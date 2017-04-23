#include "visualizer.h"

/* ================================================================== *
 * Function: decay
 * Description: slowly fades out matrix values
 * Parameters: [float] factor - decay factor to be multiplied by
 * ================================================================== */
void Visualizer::decay(double factor) {
    for (char i = 0; i < NUM_BARS; i++) {
        for (char j = 0; j < BAR_HEIGHT; j++) {
            unsigned int color = bars[i]->getPixelColor(j);
            bars[i]->setPixelColor(j, (char)(color >> 16) * factor,
            (char)(color >> 8) * factor,
            (char)(color) * factor);
        }
    }
}

/* ================================================================== *
 * Function: decay_to_rgb
 * Description: slowly fades out matrix values to the specified rgb value
 * Parameters: [float] factor - decay factor to be multiplied by
 *             [int] r - the red value to fade to
 *             [int] g - the green value to fade to
 *             [int] b - the blue value to fade to
 * ================================================================== */
void Visualizer::decay_to_rgb(double factor, int r, int g, int b) {
    for (char i = 0; i < NUM_BARS; i++) {
        for (char j = 0; j < BAR_HEIGHT; j++) {
            unsigned int color = bars[i]->getPixelColor(j);

            // Decay and do bounds checking on rgb values
            int rx = (int) ((char)(color >> 16) * factor);
            int gx = (int) ((char)(color >>  8) * factor);
            int bx = (int) ((char)(color      ) * factor);
            if (rx < r) { rx = r; }
            if (gx < g) { gx = g; }
            if (bx < b) { bx = b; }

            bars[i]->setPixelColor(j, rx, gx, bx);
        }
    }
}

void Visualizer::fill_bar(int bar, int r, int g, int b) {
    for (int j =0; j < BAR_HEIGHT; j++) {
        bars[bar]->setPixelColor(j, (uint8_t)r, (uint8_t)g, (uint8_t)b);
    }
}

/* ================================================================== *
 * Function: fill_matrix
 * Description: Sets all pixel values in matrix to given color value
 * Parameters: [Color]* color - color to set matrix to
 * ================================================================== */
void Visualizer::fill_matrix(Color_Value c) {
    for (int i = 0; i < NUM_BARS; i++) {
        for (int j =0; j< BAR_HEIGHT; j++) {
            bars[i]->setPixelColor(j, c.c);
        }
    }
}

/* ================================================================== *
 * Function: init_matrix
 * Description: Initializes the matrix to off
 * ================================================================== */
void Visualizer::init_matrix() {
    for (int i = 0; i < NUM_BARS; i++) {
        // switch height and width based on orientation
        bars[i]->begin();
        bars[i]->show();
    }
}

void Visualizer::mix_pixel(unsigned char bar, unsigned short pixel, float factor, unsigned char r, unsigned char g, unsigned char b) {
    unsigned int color = bars[bar]->getPixelColor(pixel);
    bars[bar]->setPixelColor(pixel, (char)(color >> 16) * (1-factor) + r * factor,
    (char)(color >> 8) * (1-factor) + g * factor,
    (char)(color) * (1-factor) + b * factor);
}

/* =================== PUBLIC MANAGEMENT FUNCTIONS ================== */

/* ================================================================== *
 * Function: clear_matrix
 * Description: Sets all pixel values to (0, 0, 0)
 * ================================================================== */
void Visualizer::clear_matrix() {
    for (int i = 0; i < NUM_BARS; i++) {
        for (int j =0; j< BAR_HEIGHT; j++) {
            bars[i]->setPixelColor(j, 0);
        }
    }
}

/* ================================================================== *
 * Function: show_all
 * Description: drives pixels in physical matrix to current values
 * ================================================================== */
void Visualizer::show_all() {
    for (char i = 0; i < NUM_BARS; i++) {
        bars[i]->show();
    }
}
