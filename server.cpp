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

//static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#define PREFIX ""
WebServer webserver(PREFIX, 80);

static void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool);

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
    webserver.setDefaultCommand(&helloCmd);

    /* run the same command if you try to load /index.html, a common
     * default page name */
    webserver.addCommand("index.html", &helloCmd);

    /* start the webserver */
    webserver.begin();

    // Print the IP so we know where to connect
    Particle.variable("ipAddress", myIpAddress, STRING);
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

    /* process incoming connections one at a time forever */
    webserver.processConnection(buff, &len);
}

/* ================================================================== *
 * Function: helloCmd
 * Description: Defualt callback function for the WebServer to run
 * Parameters: none
 * ================================================================== */
static void helloCmd(WebServer &server, WebServer::ConnectionType type, char * c, bool b)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    P(helloMsg) = "<h1>Hello, World!</h1>";

    /* this is a special form of print that outputs from PROGMEM */
    server.printP(helloMsg);
  }
}
