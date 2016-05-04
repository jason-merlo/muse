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

int static_red = 0;            //integer for red darkness
int static_blue = 0;           //integer for blue darkness
int static_green = 0;          //integer for green darkness
int static_visualizer_type = VISUALIZER_BARS;

void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool);
void rgbCmd(WebServer &server, WebServer::ConnectionType type, char *, bool);
void web_index(WebServer &server, WebServer::ConnectionType type, char * c, bool b);
void web_input(WebServer &server, WebServer::ConnectionType type, char * c, bool b);

/* ================================================================== *
 * Server
 * ================================================================== */
Server::Server() {}

/* ================================================================== *
 * Function: init
 * Description: Initialize the server
 * Parameters: none
 * ================================================================== */
void Server::init() {
    /* setup our default command that will be run when the user accesses
     * the root page on the server */
    webserver.setDefaultCommand(&web_index);

    /* run the same command if you try to load /index.html, a common
     * default page name */
    webserver.addCommand("web_input", &web_input);

    /* start the webserver */
    webserver.begin();

    // Print the IP so we know where to connect
    server_red = 0;

    Particle.variable("ipAddress", myIpAddress, STRING);
    Particle.variable("RedValue", &server_red, INT);
    IPAddress myIp = WiFi.localIP();
    sprintf(myIpAddress, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
}

/* ================================================================== *
 * Function: tick
 * Description: Periodically call to serve HTTP connections
 * Parameters: none
 * ================================================================== */
void Server::tick() {
    char buff[256];
    int len = 256;

    server_red = static_red;
    visualizer_type = static_visualizer_type;

    /* process incoming connections one at a time forever */
    webserver.processConnection(buff, &len);
}

int Server::visualizer() {
    return visualizer_type;
}

void web_index(WebServer &server, WebServer::ConnectionType type, char * c, bool b) {
    server.httpSuccess();

    if (type != WebServer::HEAD) {
      server.printP(control_panel);
    }
}

void web_input(WebServer &server, WebServer::ConnectionType type, char * c, bool b) {
    if (type == WebServer::POST) {
        bool repeat;
        char name[16], value[16];
        do {
            // Read all POST params, returns false when no more params
            repeat = server.readPOSTparam(name, 16, value, 16);

            if (strcmp(name, "visualizer") == 0) {
                int type = strtol(value, NULL, 10);
                switch (type) {
                    case VISUALIZER_BARS:
                        static_visualizer_type = VISUALIZER_BARS;
                        break;
                    case VISUALIZER_BARS_MIDDLE:
                        static_visualizer_type = VISUALIZER_BARS_MIDDLE;
                        break;
                    case VISUALIZER_PULSE:
                        static_visualizer_type = VISUALIZER_PULSE;
                        break;
                    case VISUALIZER_PLASMA:
                        static_visualizer_type = VISUALIZER_PLASMA;
                        break;
                    case VISUALIZER_RAINBOW:
                        static_visualizer_type = VISUALIZER_RAINBOW;
                        break;
                    case VISUALIZER_WHEEL:
                        static_visualizer_type = VISUALIZER_WHEEL;
                        break;
                    default: static_visualizer_type = VISUALIZER_BARS; break;
                }
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

/* ================================================================== *
 * Function: helloCmd
 * Description: Defualt callback function for the WebServer to run
 * Parameters: none
 * ================================================================== */
void helloCmd(WebServer &server, WebServer::ConnectionType type, char * c, bool b)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this is a special form of print that outputs from PROGMEM */
    server.printP(rgb_main_html);
  }
}

/* This command is set as the default command for the server.  It
 * handles both GET and POST requests.  For a GET, it returns a simple
 * page with some buttons.  For a POST, it saves the value posted to
 * the red/green/blue variable, affecting the output of the speaker */
void rgbCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  if (type == WebServer::POST)
  {
    bool repeat;
    char name[16], value[16];
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);

      /* this is a standard string comparison function.  It returns 0
       * when there's an exact match.  We're looking for a parameter
       * named red/green/blue here. */
      if (strcmp(name, "red") == 0)
      {
	/* use the STRing TO Unsigned Long function to turn the string
	 * version of the color strength value into our integer red/green/blue
	 * variable */
        static_red = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "green") == 0)
      {
        static_green = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "blue") == 0)
      {
        static_blue = strtoul(value, NULL, 10);
      }
    } while (repeat);

    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method.
    server.httpSeeOther(PREFIX);

    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
    server.printP(rgb_main_html);
  }
}
