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
 *
 * num_bars - number of LED bars
 * bar_len - number of leds in each bar (strip length)
 * orientation - horizontal or vertical bars (horizontal/ladder = 0; vertical/fence = 1)
 * ================================================================== */
Server::Server() {
}

void Server::init() {

    /* setup our default command that will be run when the user accesses
     * the root page on the server */
     //void (*hw)(WebServer &server, WebServer::ConnectionType type, char *, bool)  = &helloCmd;
    //  void (Server::*hw)(WebServer &server, WebServer::ConnectionType type, char *, bool)
    //     = (std::function<void(WebServer &server, WebServer::ConnectionType type, char *, bool)>)
    //     std::bind(&Server::helloCmd, this, _1);
        //hw = &Server::helloCmd;
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

char buff[256];
int len = 256;

void Server::tick() {
    /* process incoming connections one at a time forever */
    webserver.processConnection(buff, &len);
    /*
    // Serve webpage
    #if ENABLE_WEB_SERVER
    if (tcp_client.connected()){// && tcp_client.available()) {
        #if ENABLE_SERIAL
        Serial.printf("Serving webpage\n");
        #endif
        serve_webpage();
    } else {
        tcp_client = tcp_server.available();
    }
    #endif
    */
}

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
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
