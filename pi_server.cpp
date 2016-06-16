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
 * Description: Initialize the server
 * Parameters: none
 * ================================================================== */
int last_byte_out;
int bytes_read;
int total_time_out;
unsigned long total_time;
unsigned long last_total_time;
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

    power_status = PI_SERVER_POWER_OFF;
    visualizer_type = VISUALIZER_BARS;

    // Debug infos
    Particle.variable("LastByte", &last_byte_out, INT);
    Particle.variable("BytesRead", &bytes_read, INT);
    Particle.variable("BitsRead", &bits_read, INT);
    Particle.variable("time", &total_time_out, INT);
    last_byte_out = 0;
    bytes_read = 0;
    total_time_out = 0;
    total_time = 0;
    last_total_time = 0;
}

/* ================================================================== *
 * Function: tick
 * Description: Periodically call to serve HTTP connections
 * Parameters: none
 * ================================================================== */
void PiServer::tick() {
    unsigned long now = micros();

    while(micros() - now < 50) {
        if (digitalRead(pi_data_ready) != data_ready_value) {
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
            }
        }
    }
    total_time += micros() - now;

    if (millis() - last_total_time > 10000) {
        last_total_time = millis();
        total_time_out = (int)total_time;
        total_time = 0;
    }
}

/* ================================================================== *
 * Function: set_power
 * Description: Sets wether Server.powered_on() should return PI_SERVER_POWER_ON or PI_SERVER_POWER_OFF
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
 * Description: Sets the visualizer to be returned by Server.visualizer()
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
