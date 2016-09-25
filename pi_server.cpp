/* ================================================================== *
 *  Written by Steven Ploog
 *
 *  updates:
 *
 *  File: pi_server.cpp
 *
 *  Description: Class to run the pi server
 * ================================================================== */

#include "pi_server.h"

#include "muse.h"

// The max number of micros to spend per tick attempting to get data
#define BYTE_SCAN_MICROS 50
// Max number of millis to wait before resetting data connection
#define TIMEOUT_MILLIS 500

// Pi communication pins
static const char pi_data_ready     = A5;
static const char pi_data           = A6;
static const char pi_data_rec       = A7;

/* ================================================================== *
 * Server constructor
 * ================================================================== */
PiServer::PiServer() {}

/* ================================================================== *
 * Function: init
 * Description: Initialize the pi server
 * Parameters: none
 * ================================================================== */
int last_byte_out;
int bytes_read;
int time_outs;
unsigned long byte_start_time;
void PiServer::init() {
    // Initialize input pins
    pinMode(pi_data_ready, INPUT);
    pinMode(pi_data, INPUT);
    pinMode(pi_data_rec, OUTPUT);

    digitalWrite(pi_data_rec, LOW);

    // Initializer variables
    data_ready_value = LOW;
    data_rec_value = LOW;

    bits_read = 0;
    incoming_byte = 0;
    last_byte = 0;
    byte_start_time = 0;

    power_status = PI_SERVER_POWER_OFF;
    visualizer_type = VISUALIZER_BARS;

    // Debug infos
    Particle.variable("LastByte", &last_byte_out, INT);
    Particle.variable("BytesRead", &bytes_read, INT);
    Particle.variable("BitsRead", &bits_read, INT);
    Particle.variable("TimeOuts", &time_outs, INT);
    last_byte_out = 0;
    bytes_read = 0;
    time_outs = 0;
}

/* ================================================================== *
 * Function: tick
 * Description: Periodically call to get data from the Pi and process it
 * Parameters: none
 * ================================================================== */
void PiServer::tick() {
    if (bits_read > 0 && millis() - byte_start_time > TIMEOUT_MILLIS) {
        time_outs++;
        byte_start_time = millis();
        bits_read = 0;
        incoming_byte = 0;
        data_rec_value = LOW;
        data_ready_value = LOW;
        last_byte_out = (int) last_byte;
    }

    // Get a byte, process if a full byte was recieved
    if (get_byte()) {
        switch (last_byte) {
            case VISUALIZER_BARS:
            case VISUALIZER_BARS_MIDDLE:
            case VISUALIZER_CLASSIC:
            case VISUALIZER_PLASMA:
            case VISUALIZER_PULSE:
            case VISUALIZER_RAINBOW:
            case VISUALIZER_WHEEL:
            case AMBIENT_LIGHTING:
            case BAR_TEST:
            case BOUNCING_LINES:
            case PIXEL_TEST:
            case SNAKE_LINES:
                set_visualizer(last_byte);
                break;
            case PI_POWER_ON_MSG:
                set_power(PI_SERVER_POWER_ON);
                break;
            case PI_POWER_OFF_MSG:
                set_power(PI_SERVER_POWER_OFF);
                break;
            default:
                // Reset communication by default
                bits_read = 0;
                incoming_byte = 0;

                data_rec_value = LOW;
                data_ready_value = LOW;

                last_byte_out = (int) last_byte;
                break;
        }
    }
}

/* ================================================================== *
 * Function: get_byte
 * Description: Attempts to get a byte from the Pi for BYTE_SCAN_MICROS microseconds
 * Parameters: none
 * Returns: true - If a full byte was received within BYTE_SCAN_MICROS microseconds
 *          false - otherwise
 * ================================================================== */
bool PiServer::get_byte() {
    unsigned long start_micros = micros();
    while(micros() - start_micros < BYTE_SCAN_MICROS && micros() > start_micros) {
        if (digitalRead(pi_data_ready) != data_ready_value) {
            if (bits_read == 0) {
                byte_start_time = millis();
            }

            // Flip the data ready value we are looking for
            if (data_ready_value == LOW) data_ready_value = HIGH;
            else data_ready_value = LOW;

            // Grab the next bit
            int next_bit = digitalRead(pi_data);

            // Flip the data rec value so the pi will send next bit
            if (data_rec_value == LOW) data_rec_value = HIGH;
            else data_rec_value = LOW;
            digitalWrite(pi_data_rec, data_rec_value);

            // Update the incoming byte
            incoming_byte = incoming_byte >> 1;
            if (next_bit) incoming_byte += 0x80;

            // Update location within byte
            bits_read++;
            if (bits_read >= 8) {
                last_byte = incoming_byte;
                bits_read = 0;
                incoming_byte = 0;

                data_rec_value = LOW;
                data_ready_value = LOW;

                last_byte_out = (int) last_byte;
                bytes_read++;

                byte_start_time = millis();
                return true; // Only get one byte at a time
            }
        }
    }

    return false;
}

/* ================================================================== *
 * Function: set_power
 * Description: Sets wether PiServer.powered_on() should return PI_SERVER_POWER_ON or PI_SERVER_POWER_OFF
 * Parameters: int onOff - Pass in PI_SERVER_POWER_ON for on, PI_SERVER_POWER_OFF for off
 * ================================================================== */
void PiServer::set_power(int onOff) {
    /* Check to make sure legal values are being passed, default to PI_SERVER_POWER_OFF */
    if (onOff == PI_SERVER_POWER_ON || onOff == PI_SERVER_POWER_OFF) {
        power_status = onOff;
    } else {
        power_status = PI_SERVER_POWER_OFF;
    }
}

/* ================================================================== *
 * Function: set_visualizer
 * Description: Sets the visualizer to be returned by PiServer.visualizer()
 * Parameters: int type - One of the visualizer constants from muse.h
 * ================================================================== */
void PiServer::set_visualizer(int type) {
    visualizer_type = type;
}

/* ================================================================== *
 * Function: powered_on
 * Description: Returns wether the bars should be on or off
 * Returns: PI_SERVER_POWER_OFF or PI_SERVER_POWER_ON
 * ================================================================== */
int PiServer::powered_on() {
    return power_status;
}

/* ================================================================== *
 * Function: visualizer
 * Description: Returns the currently selected visualizer
 * Returns: The type of visualizer that was last set by set_visualizer()
 * ================================================================== */
int PiServer::visualizer() {
    return visualizer_type;
}
