/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  updates:
 *  11/17/2015
 *
 *  File: bar_matrix.h
 *
 *  Description: Class declarations for bar matrix
 * ================================================================== */

#ifndef BAR_MATRIX_H
#define BAR_MATRIX_H

#include "application.h"
#include "beat_detection.h"
#include "neopixel.h"
#include "muse.h"
#include "tcp_beats.h"

#include "math.h"

#include "bars.h"
#include "bars_middle.h"
#include "bass_middle.h"
#include "bass_slide.h"
#include "bouncing_lines.h"
#include "classic.h"
#include "plasma.h"
#include "pong.h"
#include "pulse.h"
#include "rainbow.h"
#include "snake.h"

// constants
#define PI 3.141592

class Bar_Matrix {
    public:
        Bar_Matrix(short num_bars, short bar_len, char led_type, const char* pins, Beat_Detection * beat_detection, audio_bins* bins);

        // General functions
        void
            clear_matrix(),
            tick(audio_bins * bins, int visualizer_type),
            show_all();

        // Misc drawing functions
        void
            ambient_lighting(Color_Value color),
            bar_test(),
            pixel_test();

    private:

        TCPBeats tcpBeats;

        audio_bins* bins;

        void init_matrix();

        // Visualizer references
        Bars        * visualizerBars;
        BarsMiddle  * barsMiddle;
        BassMiddle  * bassMiddle;
        BassSlide   * bassSlide;
        BouncingLines * bouncingLines;
        Classic     * classic;
        Plasma      * plasma;
        Pong        * pong;
        Pulse       * pulse;
        Rainbow     * rainbow;
        Snake       * snake;
};

#endif // BAR_MATRIX_H
