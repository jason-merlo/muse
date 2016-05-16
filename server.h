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

#define SERVER_POWER_ON  1
#define SERVER_POWER_OFF 0

class Server {
    public:
        Server();

        void init();
        void tick();

        void set_power(int onOff);
        void set_visualizer(int type);

        int powered_on();
        int visualizer();

    private:
        char myIpAddress[24];
        int power_status;
        int visualizer_type;
        int server_red;
        unsigned long last_dns_advert;
};

#endif
