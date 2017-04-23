/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  updates:
 *  11/17/2015
 *
 *  File: bar_matrix.cpp
 *
 *  Description: Class to control LED bar matrix and run visualizers
 * ================================================================== */

#include "bar_matrix.h"
#include "math.h"

// Display variables
short disp_width;
short disp_height;
unsigned long** display;
Adafruit_NeoPixel** bars;

Beat_Detection * bd;

/* ================================================================== *
 * Bar_matrix
 *
 * num_bars - number of LED bars
 * bar_len - number of leds in each bar (strip length)
 * orientation - horizontal or vertical bars (horizontal/ladder = 0; vertical/fence = 1)
 * ================================================================== */
Bar_Matrix::Bar_Matrix(short num_bars, short bar_len, const char led_type, const char* pins, Beat_Detection* beat_detection, audio_bins* bins) {
    disp_width = num_bars; //(orientation) ? num_bars : bar_len;
    disp_height = bar_len; //(orientation) ? bar_len : num_bars;

    display = new unsigned long*[disp_width];
    for (int i = 0; i < disp_width; i++)
    display[i] = new unsigned long[disp_height];

    bars = new Adafruit_NeoPixel*[num_bars];
    for(short i = 0; i < num_bars; i++) {
        bars[i] = new Adafruit_NeoPixel(bar_len, pins[i], led_type);
    }

    init_matrix();
    clear_matrix();

    bd = beat_detection;
    this->bins = bins;

    tcpBeats.init();

    visualizerBars  = new Bars          (bars, bins, bd, 0.15, 0.85);
    barsMiddle      = new BarsMiddle    (bars, bins, bd, 0.15, 0.85);
    bassMiddle      = new BassMiddle    (bars, bins, bd, 0.15, 0.80);
    bassSlide       = new BassSlide     (bars, bins, bd, 0.15, 0.75);
    bouncingLines   = new BouncingLines (bars, bins, bd, 0.00, 0.00);
    classic         = new Classic       (bars, bins, bd, 0.15, 0.90);
    pulse           = new Pulse         (bars, bins, bd, 0.15, 0.80);
    plasma          = new Plasma        (bars, bins, bd, 0.50, 0.965);
    rainbow         = new Rainbow       (bars, bins, bd, 0.15, 0.80);
    pong            = new Pong          (bars, bins, bd, 0.965, 0.0);
    snake           = new Snake         (bars, bins, bd, 0.00, 0.00);
}

/* ======================== PRIVATE FUNCTIONS ======================= */

/* ================================================================== *
 * Function: ambient_lighting
 * Description: Sets all pixel values in matrix to given color value
 * Parameters: [Color]* color - color to set matrix to
 * ================================================================== */
void Bar_Matrix::ambient_lighting(Color_Value c) {
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
void Bar_Matrix::init_matrix() {
    for (int i = 0; i < disp_width; i++) {
        // switch height and width based on orientation
        bars[i]->begin();
        bars[i]->show();
    }
}

/* =================== PUBLIC MANAGEMENT FUNCTIONS ================== */

/* ================================================================== *
 * Function: clear_matrix
 * Description: Sets all pixel values to (0, 0, 0)
 * ================================================================== */
void Bar_Matrix::clear_matrix() {
    for (int i = 0; i < disp_width; i++) {
        for (int j =0; j< disp_height; j++) {
            bars[i]->setPixelColor(j, 0);
        }
    }
}

/* ================================================================== *
 * Function: show_all
 * Description: drives pixels in physical matrix to current values
 * ================================================================== */
void Bar_Matrix::show_all() {
    for (char i = 0; i < disp_width; i++) {
        bars[i]->show();
    }
}

/* ================================================================== *
 * Function: tick
 * Description: Draws the given visualizer to the bars
 * Parameters: [audio_bins *] bins - The audio sample bins
 *             [int] visualizer_type - The visualizer type to drive
 * ================================================================== */
void Bar_Matrix::tick(audio_bins * bins, int visualizer_type) {
    switch (visualizer_type) {
        case VISUALIZER_BARS:           visualizerBars->tick(); break;
        case VISUALIZER_BARS_MIDDLE:    barsMiddle->tick();     break;
        case VISUALIZER_BASS_MIDDLE:    bassMiddle->tick();     break;
        case VISUALIZER_BASS_SLIDE:     bassSlide->tick();      break;
        case VISUALIZER_CLASSIC:        classic->tick();        break;
        case VISUALIZER_PLASMA:         plasma->tick();         break;
        case VISUALIZER_PONG:           pong->tick();           break;
        case VISUALIZER_PULSE:          pulse->tick();          break;
        case VISUALIZER_RAINBOW:        rainbow->tick();        break;

        case BOUNCING_LINES:    bouncingLines->tick();  break;
        case SNAKE_LINES:       snake->tick();          break;

        case BAR_TEST:
          bar_test();
          break;
        case PIXEL_TEST:
          pixel_test();
          break;
        case AMBIENT_LIGHTING:
          ambient_lighting(Color_Value(255,0,0));//(200, 150, 100));
          break;
    }

    bd->frame_ticked();
    show_all();
}

/* ===================== MISC DRAWING FUNCTIONS ===================== */

/* ================================================================== *
 * Function: bar_test
 * Description: Turns on bars in order to test wiring
 * Parameters: None
 * ================================================================== */
void Bar_Matrix::bar_test() {
    for (int i = 0; i < STRIP_LENGTH; i++)
    bars[int(millis()/1000)%8]->setPixelColor(i, 64, 64, 64);
}

/* ================================================================== *
 * Function: bar_test
 * Description: Turns on lights from top to check length of srip
 * Parameters: None
 * ================================================================== */
void Bar_Matrix::pixel_test() {
    clear_matrix();
    for (int i = 0; i < NUM_BARS; i++)
    bars[i]->setPixelColor(STRIP_LENGTH-int(millis()/1000)%10, 64, 64, 64);
}
