/* ================================================================== *
 *  Written by Steven Ploog
 *
 *  updates:
 *
 *  File: server.cpp
 *
 *  Description: Class to run the http server
 * ================================================================== */

#include "server.h"

#include "muse.h"
#include "webpage.h"
#include "WebServer.h"

#define PREFIX ""
WebServer webserver(PREFIX, 80);

/* Callbacks for the webserver. Serve pages and handle commands. */
void web_index(WebServer &server, WebServer::ConnectionType type, char * c, bool b, void * obj);
void web_input(WebServer &server, WebServer::ConnectionType type, char * c, bool b, void * obj);

/* ================================================================== *
 * Server constructor
 * ================================================================== */
Server::Server() {}

/* ================================================================== *
 * Function: init
 * Description: Initialize the server
 * Parameters: none
 * ================================================================== */
void Server::init() {
    /* Default command accessed through http://ip_address */
    webserver.setDefaultCommand(&web_index);

    /* Command to handle web inputs, accessed through http://ip_address/web_input */
    webserver.addCommand("web_input", &web_input, this);

    /* start the webserver */
    webserver.begin();

    // Print the IP so we know where to connect
    Particle.variable("ipAddress", myIpAddress, STRING);
    IPAddress myIp = WiFi.localIP();
    sprintf(myIpAddress, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);

    power_status = SERVER_POWER_ON;
}

/* ================================================================== *
 * Function: tick
 * Description: Periodically call to serve HTTP connections
 * Parameters: none
 * ================================================================== */
void Server::tick() {
    char buff[256];
    int len = 256;

    /* process incoming connections one at a time forever */
    webserver.processConnection(buff, &len);
}

/* ================================================================== *
 * Function: set_power
 * Description: Sets wether Server.powered_on() should return SERVER_POWER_ON or SERVER_POWER_OFF
 * Parameters: int onOff - Pass in SERVER_POWER_ON for on, SERVER_POWER_OFF for off
 * ================================================================== */
void Server::set_power(int onOff) {
    /* Check to make sure legal values are being passed, default to SERVER_POWER_OFF */
    if (onOff == SERVER_POWER_ON || onOff == SERVER_POWER_OFF) {
        power_status = onOff;
    } else {
        power_status = SERVER_POWER_OFF;
    }
}

/* ================================================================== *
 * Function: set_visualizer
 * Description: Sets the visualizer to be returned by Server.visualizer()
 * Parameters: int type - One of the visualizer constants from muse.h
 * ================================================================== */
void Server::set_visualizer(int type) {
    visualizer_type = type;
}

/* ================================================================== *
 * Function: powered_on
 * Description: Returns wether the bars should be on or off
 * Returns: SERVER_POWER_OFF or SERVER_POWER_ON
 * ================================================================== */
int Server::powered_on() {
    return power_status;
}

/* ================================================================== *
 * Function: visualizer
 * Description: Returns the currently selected visualizer
 * Returns: The type of visualizer that was last set by set_visualizer()
 * ================================================================== */
int Server::visualizer() {
    return visualizer_type;
}

/* ================================================================== *
 * Function: web_index
 * Description: Static callback function to display the homepage of the web server
 * Parameters: See Webduino documentation
 *             obj is a pointer to the instance of Server that added the callback
 * ================================================================== */
void web_index(WebServer &server, WebServer::ConnectionType type, char * c, bool b, void * obj) {
    server.httpSuccess();

    if (type != WebServer::HEAD) {
      server.printP(control_panel);
    }
}

/* ================================================================== *
 * Function: web_input
 * Description: Static callback function to handle input to the server
 * Parameters: See Webduino documentation
 *             obj is a pointer to the instance of Server that added the callback
 * ================================================================== */
void web_input(WebServer &server, WebServer::ConnectionType type, char * c, bool b, void * obj) {
    if (type == WebServer::POST) {
        Server * s = (Server *) obj;
        bool repeat;
        char name[16], value[16];
        do {
            // Read all POST params, returns false when no more params
            repeat = server.readPOSTparam(name, 16, value, 16);

            if (strcmp(name, "visualizer") == 0) {
                int type = strtol(value, NULL, 10);
                // Ensure type is valid, default to VISUALIZER_BARS
                switch (type) {
                    case VISUALIZER_BARS:
                    case VISUALIZER_BARS_MIDDLE:
                    case VISUALIZER_PULSE:
                    case VISUALIZER_PLASMA:
                    case VISUALIZER_RAINBOW:
                    case VISUALIZER_WHEEL:
                        s->set_visualizer(type); break;
                    default:
                        s->set_visualizer(VISUALIZER_BARS); break;
                }
            } else if (strcmp(name, "other") == 0) {
                int type = strtol(value, NULL, 10);
                // Ensure type is valid, default to BOUNCING_LINES
                switch (type) {
                    case BOUNCING_LINES:
                    case BAR_TEST:
                    case PIXEL_TEST:
                    case AMBIENT_LIGHTING:
                        s->set_visualizer(type); break;
                    default:
                        s->set_visualizer(BOUNCING_LINES); break;
                }
            } else if (strcmp(name, "power") == 0) {
                s->set_power(strtol(value, NULL, 10));
            }
        } while (repeat);
            // after procesing the POST data, tell the web browser to reload
            // the page using a GET method.
            server.httpSeeOther("/web_input");
            return;
        }

        /* for a GET or HEAD, send the standard "it's all OK headers" */
        server.httpSuccess();

        /* we don't output the body for a HEAD request */
        if (type == WebServer::GET) {
            server.printP(control_panel);
        }
}
