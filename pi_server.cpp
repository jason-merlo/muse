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

    // Begin UDP socket
    udp.begin(1738);

    for (int i = 0; i < PI_MAX_MSG_LEN; i++) {
        message[i] = 0;
    }

    power_status = PI_SERVER_POWER_OFF;
    visualizer_type = VISUALIZER_BARS;

    // Debug infos
    Particle.variable("LastMsg", &last_msg, INT);
    Particle.variable("MsgsRead", &num_msgs, INT);
    num_msgs = 0;
    last_msg = 0;
}

/* ================================================================== *
 * Function: tick
 * Description: Periodically call to get data from the Pi and process it
 * Parameters: none
 * ================================================================== */
void PiServer::tick() {
    // Get a message, process if a full byte was recieved
    if (get_msg()) {
        switch (message[1]) {
            case VISUALIZER_BARS:
            case VISUALIZER_BARS_MIDDLE:
            case VISUALIZER_BASS_MIDDLE:
            case VISUALIZER_BASS_SLIDE:
            case VISUALIZER_CLASSIC:
            case VISUALIZER_PLASMA:
            case VISUALIZER_PONG:
            case VISUALIZER_PULSE:
            case VISUALIZER_RAINBOW:
            case VISUALIZER_WHEEL:
            case AMBIENT_LIGHTING:
            case BAR_TEST:
            case BOUNCING_LINES:
            case PIXEL_TEST:
            case SNAKE_LINES:
                set_visualizer(message[1]);
                break;
            case PI_POWER_ON_MSG:
                set_power(PI_SERVER_POWER_ON);
                break;
            case PI_POWER_OFF_MSG:
                set_power(PI_SERVER_POWER_OFF);
                break;
            default:
                // Do nothing for default
                break;
        }
    }
}

/* ================================================================== *
 * Function: get_msg
 * Description: Attempts to get a message from the Pi
 * Parameters: none
 * Returns: true - If a message was received
 *          false - otherwise
 * ================================================================== */
bool PiServer::get_msg() {

    // Required for udp.available/read
    udp.parsePacket();

    // Check if we have enough data, if we do then process message
    if (udp.available() >= PI_MSG_LEN) {
        message[0] = udp.read();
        message[1] = udp.read();
        message[2] = udp.read();

        return is_valid_msg();
    } else {
        // Else flush unneeded bytes
        while (udp.read() != -1);
    }

    return false;
}

/* ================================================================== *
 * Function: is_valid_message
 * Description: Checks if a message is valid
 * Parameters: none
 * Returns: true - message contains a valid message
 *          false - otherwise
 * ================================================================== */
bool PiServer::is_valid_msg() {
    if (message[0] == PI_MSG_START && message[2] == PI_MSG_END) {
        num_msgs++;
        last_msg = message[1];
        return true;
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
