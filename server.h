/* ================================================================== *
 *  Written by Steven Ploog
 *
 *  updates:
 *
 *  File: server.h
 *
 *  Description: Header for http server
 * ================================================================== */

#ifndef SERVER_H
#define SERVER_H

#include "application.h"
#include "WebServer.h"
//#include "webpage.h"

class Server {
    public:
        Server();
        void init();

        void tick();

    private:
        char myIpAddress[24];
};

#endif
