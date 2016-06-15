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

/* ================================================================== *
 * Server constructor
 * ================================================================== */
PiServer::Server() {}

/* ================================================================== *
 * Function: init
 * Description: Initialize the server
 * Parameters: none
 * ================================================================== */
void PiServer::init() {
    power_status = PI_SERVER_POWER_ON;
}

/* ================================================================== *
 * Function: tick
 * Description: Periodically call to serve HTTP connections
 * Parameters: none
 * ================================================================== */
void PiServer::tick() {

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
