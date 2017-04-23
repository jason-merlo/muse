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

#include "bars.h"
#include "bars_middle.h"
#include "bass_middle.h"
#include "bass_slide.h"
#include "classic.h"
#include "plasma.h"
#include "rainbow.h"

// Display variables
short disp_width;
short disp_height;
unsigned long** display;
Adafruit_NeoPixel** bars;

Beat_Detection * bd;

int color_table_idx = 0;

Bars        * visualizerBars;
BarsMiddle  * visualizerBarsMiddle;
BassMiddle  * visualizerBassMiddle;
BassSlide   * visualizerBassSlide;
Classic     * visualizerClassic;
Plasma      * visualizerPlasma;
Rainbow     * visaulizerRainbow;

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

        // Set up variables to track bouncing lines
        bouncing_line_lengths[i] = random(10, STRIP_LENGTH / 2);
        bouncing_line_positions[i] = random(0, STRIP_LENGTH);
        //bouncing_line_positions[i] = (i%2 == 0) ? 0 : STRIP_LENGTH;
        if (random(0, 10) > 5) bouncing_line_directions[i] = 1;
        else                bouncing_line_directions[i] = -1;

        bouncing_line_speeds[i] = (float)random(5, 10) / 15.0;

        bouncing_line_colors[i][0] = bd->r();
        bouncing_line_colors[i][1] = bd->g();
        bouncing_line_colors[i][2] = bd->b();
    }

    color_table_idx = 0;
    snakes[0] = Snake(1, 30, -30, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;
    snakes[1] = Snake(-1, 25, 4*70, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;
    snakes[2] = Snake(1, 20, 185, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;
    snakes[3] = Snake(-1, 15, 8*70, COLOR_TABLE[color_table_idx][0], COLOR_TABLE[color_table_idx][1], COLOR_TABLE[color_table_idx][2]);
    color_table_idx++;

    snakes[0].posTicks = 0;
    snakes[0].ticksNeeded = 1;
    snakes[1].posTicks = 0;
    snakes[1].ticksNeeded = 1;
    snakes[2].posTicks = 0;
    snakes[2].ticksNeeded = 1;
    snakes[3].posTicks = 0;
    snakes[3].ticksNeeded = 1;

    pongPaddles[0] = PongPaddle(15, 0, STRIP_LENGTH/2, 0, 0.9, 15, 100, 100, 100);
    pongPaddles[1] = PongPaddle(15, NUM_BARS-1, STRIP_LENGTH/2, 0, 0.9, 15, 100, 100, 100);
    pongBall = PongBall(3, 0.0625, 0.80, 15, 100, 100, 100);
    last_beat_count = 0;

    bass_slide_millis = millis();
    bass_slide_ema = 0.0;

    init_matrix();
    clear_matrix();

    bd = beat_detection;
    this->bins = bins;

    tcpBeats.init();

    visualizerBars          = new Bars      (bars, bins, bd, 0.15, 0.85);
    visualizerBarsMiddle    = new BarsMiddle(bars, bins, bd, 0.15, 0.85);
    visualizerBassMiddle    = new BassMiddle(bars, bins, bd, 0.15, 0.80);
    visualizerBassSlide     = new BassSlide (bars, bins, bd, 0.15, 0.75);
    visualizerClassic       = new Classic   (bars, bins, bd, 0.15, 0.90);
    visualizerPlasma        = new Plasma    (bars, bins, bd, 0.50, 0.965);
    visualizerRainbow       = new Rainbow   (bars, bins, bd, 0.15, 0.80);
}

/* ======================== PRIVATE FUNCTIONS ======================= */

/* ================================================================== *
 * Function: decay
 * Description: slowly fades out matrix values
 * Parameters: [float] factor - decay factor to be multiplied by
 * ================================================================== */
void Bar_Matrix::decay(double factor) {
    for (char i = 0; i < disp_width; i++) {
        for (char j = 0; j < disp_height; j++) {
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
void Bar_Matrix::decay_to_rgb(double factor, int r, int g, int b) {
    for (char i = 0; i < disp_width; i++) {
        for (char j = 0; j < disp_height; j++) {
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

void Bar_Matrix::fill_bar(int bar, int r, int g, int b) {
    for (int j =0; j < disp_height; j++) {
        bars[bar]->setPixelColor(j, (uint8_t)r, (uint8_t)g, (uint8_t)b);
    }
}

/* ================================================================== *
 * Function: fill_matrix
 * Description: Sets all pixel values in matrix to given color value
 * Parameters: [Color]* color - color to set matrix to
 * ================================================================== */
void Bar_Matrix::fill_matrix(Color_Value c) {
    for (int i = 0; i < disp_width; i++) {
        for (int j =0; j< disp_height; j++) {
            bars[i]->setPixelColor(j, c.c);
        }
    }
}

/* ================================================================== *
 * Function: ambient_lighting
 * Description: Sets all pixel values in matrix to given color value
 * Parameters: [Color]* color - color to set matrix to
 * ================================================================== */
void Bar_Matrix::ambient_lighting(Color_Value c) {
    fill_matrix(c);
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

void Bar_Matrix::mix_pixel(unsigned char bar, unsigned short pixel, float factor, unsigned char r, unsigned char g, unsigned char b) {
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
        // Visualizers
        case VISUALIZER_BARS:           visualizerBars->tick();         break;
        case VISUALIZER_BARS_MIDDLE:    visualizerBarsMiddle->tick();   break;
        case VISUALIZER_BASS_MIDDLE:    visualizerBassMiddle->tick();   break;
        case VISUALIZER_BASS_SLIDE:     visualizerBassSlide->tick();    break;
        case VISUALIZER_CLASSIC:        visualizerClassic->tick();      break;
        case VISUALIZER_PLASMA:         visualizerPlasma->tick();       break;
        case VISUALIZER_PONG:
            visualizer_pong(0.965);
            break;
        case VISUALIZER_PULSE:
            visualizer_pulse(bins, 0.15, 0.8, 1.0f, 20.0f);
            break;
        case VISUALIZER_RAINBOW:        visualizerRainbow->tick();      break;
        case VISUALIZER_WHEEL:
            visualizer_wheel(0.25, 10);
            break;

        // Tests and misc
        case BOUNCING_LINES:
          bouncing_lines();
          break;
        case SNAKE_LINES:
            snake_lines(0.75);
            break;
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
 * Function: bouncing_lines
 * Description: Bounces a solid line of LEDs up and down in each matrix bar
 * Parameters: none
 * ================================================================== */
void Bar_Matrix::bouncing_lines() {
    for (int i = 0; i < disp_width; i++) {
        int bottom = bouncing_line_positions[i];

        // Clear the pixel the line just left, light the one it entered
        if (bouncing_line_directions[i] == 1) {
            if (bottom > 0) mix_pixel(i, bottom-1, 1, 0, 0, 0);
            mix_pixel(i, 1+bottom+bouncing_line_lengths[i], 1, bouncing_line_colors[i][0], bouncing_line_colors[i][1], bouncing_line_colors[i][2]);//bd->r(), bd->g(), bd->b());
        } else {
            if (bottom > 0) mix_pixel(i, bottom-1, 1, bouncing_line_colors[i][0], bouncing_line_colors[i][1], bouncing_line_colors[i][2]);//bd->r(), bd->g(), bd->b());
            mix_pixel(i, 1+bottom+bouncing_line_lengths[i], 1, 0, 0, 0);
        }

        // Move the line
        bouncing_line_positions[i] += bouncing_line_directions[i] * bouncing_line_speeds[i];

        // Ensure we are moving in the proper direction
        if (bouncing_line_positions[i] < -.5*bouncing_line_lengths[i] && bouncing_line_directions[i] < 0) {
            bouncing_line_directions[i] = 1;
            if (random(0, 10) > 5) {
                int newlength = random(10, STRIP_LENGTH / 2);
                if (newlength > bouncing_line_lengths[i]) {
                    bouncing_line_positions[i] = (-1*newlength/2) - (newlength/2 - bouncing_line_lengths[i]/2);
                } else {
                    bouncing_line_positions[i] = -1 * newlength / 2;
                }

                bouncing_line_lengths[i] = newlength;
            }

            if (random(0, 10) > 7) {
                bouncing_line_speeds[i] = (float)random(5, 10) / 15.0;
            }

            bouncing_line_colors[i][0] = bd->r();
            bouncing_line_colors[i][1] = bd->g();
            bouncing_line_colors[i][2] = bd->b();
        } else if (bouncing_line_positions[i] > STRIP_LENGTH-.5*bouncing_line_lengths[i] && bouncing_line_directions[i] > 0) {
            bouncing_line_directions[i] = -1;

            bouncing_line_colors[i][0] = bd->r();
            bouncing_line_colors[i][1] = bd->g();
            bouncing_line_colors[i][2] = bd->b();
        }
    }
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

/* ================== VISUALIZER DRAWING FUNCTIONS ================== */

void Bar_Matrix::visualizer_pong(float in_factor) {
    pongPaddles[0].tick(&pongBall);
    pongPaddles[1].tick(&pongBall);
    pongBall.tick();

    if (bd->num_beats() != last_beat_count) {
        last_beat_count = bd->num_beats();
        if (random(0, 3) == 0 ) {
            pongPaddles[1].setColor(bd->r(), bd->g(), bd->b());
        } else if (random(0, 2) == 0) {
            pongPaddles[0].setColor(bd->r(), bd->g(), bd->b());
        } else {
            pongBall.setColor(bd->r(), bd->g(), bd->b());
        }
    }

    clear_matrix();

    // Draw paddles
    for (int i = 0; i < 2; i++) {
        for (int y = 0; y < pongPaddles[i].len; y++) {
            mix_pixel(pongPaddles[i].xPos(), pongPaddles[i].yPos()+y, in_factor, pongPaddles[i].r, pongPaddles[i].g, pongPaddles[i].b);
        }
    }

    // Draw Ball
    for (int i = 0; i < pongBall.len; i++) {
        mix_pixel(pongBall.xPos(), pongBall.yPos()+i, in_factor, pongBall.r, pongBall.g, pongBall.b);
    }
}

/* ================================================================== *
 * Function: visualizer_pulse
 * Description: Creates pulses where sound appears to be originating from
 * Parameters:  None
 * ================================================================== */
void Bar_Matrix::visualizer_pulse(audio_bins* bins, float in_factor, float out_factor, float distance_x, float distance_y) {
    decay (out_factor);

    // Calculate frequency pan and "fade"
    float pans[NUM_BINS]; // ratio left to right, 0-1
    float intensities[NUM_BINS]; // ratio low to high, 0-1
    for (char i = 0; i < NUM_BINS; i++)
    pans[i] = float(bins->left[i])/float(bins->right[i]);
    for (char i = 0; i < NUM_BINS; i++)
    intensities[i] = ((float(bins->left[i])+float(bins->right[i]))/2.0f)/BINS_MAX;

    for (char i = 0; i < disp_width; i++) {
        for (char j = 0; j < disp_height; j++) {
            float level = 0;

            // Calculate level intensity
            for (char x = 0; x < NUM_BINS; x++) {
                float distance = distance_x * 1.0f/(abs(pans[x]-(i/disp_width))) + distance_y * 1.0f/(abs((x/NUM_BINS)-(j/disp_height)));
                level += intensities[x] * distance;
            }

            //level *= 2*PI;
            //mix_pixel(i, j, in_factor, cos(level)*255*level, cos(level - 2*PI/3)*255*level, cos(level - 4*PI/3)*255*level);
            mix_pixel(i, j, in_factor, level*255, level*255, level*255);
        }
    }
}

/* ================================================================== *
 * Function: snake_lines
 * Description: Snake runs up and down each bar
 * Parameters: [float] speed - speed snake moves at
 * ================================================================== */
void Bar_Matrix::snake_lines(float speed) {
    bool noChange = true;

    fill_matrix(Color_Value(0, 0, 0));

    for (int x = 0; x < NUM_SNAKES; x++) {
        Snake *s = &snakes[x];

        for (int i = 0; i < s->len; i++) {
            int b = (s->pos + i) / 70; // bar
            int p = (s->pos + i) % 70; // pixel

            if (b % 2 == 1) {
                p = 69-p;
            }

            mix_pixel(b, p, .95, s->r, s->g, s->b);
        }

        if (noChange && random(0, 10) > 7) {
            s->r = bd->r();
            s->g = bd->g();
            s->b = bd->b();
            noChange = false;
        }

        s->posTicks++;
        if (s->posTicks >= s->ticksNeeded) {
            s->posTicks = 0;
            s->pos += s->dir;
        }
        if ((s->dir > 0 && s->pos > 8*70+s->len) ||
            (s->dir < 0 && s->pos < -s->len))
        {
            color_table_idx += random(0, 10);
            color_table_idx = color_table_idx % 50;

            s->pos = s->dir > 0 ? -s->len : 8*70;
        }
    }
}

/* ================================================================== *
 * Function: visualizer_wheel
 * Description: Sets all bars to a color while rotating through all
 *              hues in order of the rainbow
 * Parameters: [float] intensity - intensity of lights
 *             [float] speed - speed wheel rotates at
 * ================================================================== */
void Bar_Matrix::visualizer_wheel(float intensity, float speed) {
    float val = fmod(millis()/10000.0f,1.0f)*2.0f*PI;
    fill_matrix(Color_Value(cos(val)*255*intensity, cos(val - 2*PI/3)*255*intensity, cos(val - 4*PI/3)*255*intensity));
}
