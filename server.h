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

class Server {
    public:
        Server();

        void init();
        void tick();

        int visualizer();

    private:
        char myIpAddress[24];
        int server_red;
};

#endif
