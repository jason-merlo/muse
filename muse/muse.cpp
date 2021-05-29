/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  updates:
 *  11/17/2015
 *
 *  File: muse.cpp
 *
 *  Description: Main program for muse visulizer
 * ================================================================== */

/* ======================= includes ================================= */
#include "muse.h"

#include "application.h"
#include "bar_matrix.h"
#include "lib/mdns/MDNS.h"
#include "neopixel.h"
#include "server.h"

SYSTEM_MODE(AUTOMATIC);

/* ======================= Pin Decls. =============================== */

// Digital Outputs
static const char ps_on   = A4;
static const char rst     = A5;
static const char strobe  = A2;

// Pi communication pins, defined/used in pi_server
// static const char pi_data_ready     = A5;
// static const char pi_data           = A6;
// static const char pi_data_rec       = A7;

// Analog Inputs (0-4096)
static const char audio_r = A1;
static const char audio_l = A0;

// Declare memory for audio bin structure
static struct audio_bins bins;

// Declare matrix pins
static const char matrix_pins[10] = {TX, RX, D7, D6, D5, D4, D3, D2, D1, D0};
//static const char matrix_pins[8] = {D7, D6, D5, D4, D3, D2, D1, D0};

#if ENABLE_BARS
// Declare matrix variables
static Bar_Matrix* matrix;
unsigned long last_display_update;
// Beat detection instance
Beat_Detection beat_detect;
#endif


#if ENABLE_WEB_SERVER
// Declare webserver variables
Server server;
unsigned int last_server_update = 0;
#endif

#if ENABLE_MDNS
MDNS mdns;
unsigned int last_mdns_update = 0;
#endif

#if ENABLE_MSGEQ7
unsigned long last_sample_millis;
#endif

#if ENABLE_PSU_CONTROL
static bool psu_is_on = false;
#endif

#if ENABLE_SCREENSAVER || ENABLE_AUTO_SHUTDOWN
static unsigned long last_sound_seconds;
#endif

// Stat trackers for number of loop ticks and number of frames
int tick_count;
int tick_count_publish;
int frame_count;
int frame_count_publish;
unsigned long last_tick_update;

/* ================================================================== *
 *  Function: Setup
 *  Description: initialize program I/O and data structures
 * ================================================================== */

void setup() {
    // Open debug terminal
    #if ENABLE_SERIAL
    Serial.begin(9600);
    #endif

    // Enables frequency analyzer
    #if ENABLE_MSGEQ7
    pinMode(rst, OUTPUT);
    pinMode(strobe, OUTPUT);
    // pinMode(audio_l, INPUT);
    // pinMode(audio_r, INPUT);
    init_eq();
    last_sample_millis = 0;
    #endif

    // Enables power switch input for PSU and control for
    #if ENABLE_PSU_CONTROL
    pinMode(ps_on, OUTPUT);
    psu_is_on = false;
    psu_startup();
    #endif

    // Create new bar matrix inistance
    #if ENABLE_BARS
    matrix = new Bar_Matrix(NUM_BARS, STRIP_LENGTH, LED_TYPE, matrix_pins, &beat_detect);
    last_display_update = 0;
    #endif

    // Initialize screenaver variables
    #if ENABLE_AUTO_SHUTDOWN || ENABLE_SCREENSAVER
    last_sound_seconds = Time.now();
    #endif

    #if ENABLE_WEB_SERVER
    server.init();
    last_server_update = 0;
    #endif

    #if ENABLE_MDNS
    mdns.setHostname("muse");
    mdns.begin();
    last_mdns_update = 0;
    #endif

    tick_count = 0;
    last_tick_update = 0;
    frame_count = 0;
    frame_count_publish = 0;
    Particle.variable("Ticks10s", &tick_count_publish, INT);
    Particle.variable("Frames10s", &frame_count_publish, INT);

    IPAddress myIP = WiFi.localIP();
    String ipStr = String(myIP[0])+"."+String(myIP[1])+"."+String(myIP[2])+"."+String(myIP[3]);
    Particle.publish("LocalIP", ipStr, 60,PRIVATE);
}

/* ================================================================== *
 *  Function: loop
 *  Description: Contains main program
 * ================================================================== */
void loop() {
    #if ENABLE_SERIAL
    Serial.printf("%d, %d, %d, %d\n", bins.left[0], bins.left[1], bins.right[0], bins.right[1]);
    #endif

    powered_on_tick();

    // Check each bin to see if they are below the threshold to be "off"
    // If any bin is active break and just run the visualizer
    #if ENABLE_PSU_CONTROL
        // Handle auto shutdown
        #if ENABLE_AUTO_SHUTDOWN
        for (int i = 0; i < NUM_BINS; i++) {
            if (bins.right[i] > SCREENSAVER_MINIMUM || bins.left[i] > SCREENSAVER_MINIMUM) {
                if (!psu_is_on) { psu_startup(); }
                last_sound_seconds = Time.now();
                break;
            }
        }
        if (psu_is_on) {
        }
        if (Time.now()-last_sound_seconds > SCREENSAVER_SECS_TO_PSU_OFF) {
            // If we have passed the seconds until psu shutoff, turn it off
            if (psu_is_on) { psu_shutdown(); }
        }
        #endif

        #if ENABLE_WEB_POWER
        if (server.powered_on() == SERVER_POWER_ON) {
            if (!psu_is_on) { psu_startup(); }
            if (millis() - last_display_update >= DISPLAY_UPDATE_INTERVAL) {
                last_display_update = millis();
                #if ENABLE_MSGEQ7
                sample_freq(&bins);
                #endif
            }
        } else {
            psu_shutdown();
        }
        #endif
    #endif

    #if ENABLE_MDNS
    if (millis() - last_mdns_update > MDNS_UPDATE_INTERVAL ||
        millis() - last_mdns_update < 0) {
            last_mdns_update = millis();
            mdns.processQueries();
    }
    #endif

    #if ENABLE_WEB_SERVER
    if (millis() - last_server_update > SERVER_UPDATE_INTERVAL ||
        millis() - last_server_update < 0) {
            last_server_update = millis();
            server.tick();
    }
    #endif

    tick_count++;
    if (millis() - last_tick_update >= 10000) {
        last_tick_update = millis();
        tick_count_publish = tick_count;
        frame_count_publish = frame_count;
        tick_count = 0;
        frame_count = 0;
    }

    // Delay to make updates from the cloud more responsive
    delay(1);
}

/* ================================================================== *
 *  Function: init_eq
 *  Description: Sends reset sequence to MSGEQ7's allowing for data
 *  reading to begin
 * ================================================================== */
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

/* ================================================================== *
 *  Function: sample_freq
 *  Description: Reads bins from MSGEQ7's and stores them to struct
 *  Parameters:  [audio_bins]* bins - frequency bins read from chip
 * ================================================================== */
void sample_freq(audio_bins* bins) {
    for (int i = 0; i < NUM_BINS; i++) {
        digitalWrite(strobe, LOW);
        delayMicroseconds(40); // allow for EQ mux to fully switch

        // Moving average bins with new sample
        bins->left[i]  = analogRead(audio_l) * 0.8 + bins->left[i]  * 0.2;
        bins->right[i] = analogRead(audio_r) * 0.8 + bins->right[i] * 0.2;

        digitalWrite(strobe, HIGH);
        delayMicroseconds(40); // allow for EQ mux to fully switch
    }
}

/* ================================================================== *
 *  Function: powered_on_tick
 *  Description: Samples/beat detects/updates frame as needed
 *  Parameters:  none
 * ================================================================== */
void powered_on_tick() {
    // #if ENABLE_PSU_CONTROL
    // if (!psu_is_on) { psu_startup(); }
    // #endif

    #if ENABLE_MSGEQ7
    if (millis() - last_sample_millis >= SAMPLE_UPDATE_INTERVAL) {
        last_sample_millis = millis();
        sample_freq(&bins);
        beat_detect.tick(&bins);
    }
    #endif

    #if ENABLE_BARS
    if (millis() - last_display_update >= DISPLAY_UPDATE_INTERVAL) {
        last_display_update = millis();
        #ifdef ENABLE_WEB_SERVER
        matrix->tick(&bins, server.visualizer());
        #else
        matrix->tick(&bins, STATIC_VISUALIZER);
        #endif
        frame_count++;
    }
    #endif
}

/* ================================================================== *
 *  Function: psu_startup
 *  Description: Turns the psu on
 *  Parameters:  none
 * ================================================================== */
void psu_startup() {
    #if ENABLE_PSU_CONTROL
    if (!psu_is_on) {
        matrix->clear_matrix();
        matrix->show_all();
        digitalWrite(ps_on, LOW);
    }
    psu_is_on = true;
    #endif
}

/* ================================================================== *
 *  Function: psu_shutdown
 *  Description: turns the psu off, sets psu_is_on to false
 *  Parameters:  none
 * ================================================================== */
void psu_shutdown() {
    #if ENABLE_PSU_CONTROL
    if (psu_is_on) {
        digitalWrite(ps_on, HIGH);
        matrix->clear_matrix();
        matrix->show_all();
    }
    psu_is_on = false;
    #endif
}
