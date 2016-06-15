/* ================================================================== *
 *  Written by Steven Ploog
 *
 *  updates:
 *
 *  File: pi_server.h
 *
 *  Description: Header for pi server
 * ================================================================== */

#ifndef PI_SERVER_H
#define PI_SERVER_H

#include "application.h"

#define PI_SERVER_POWER_ON  1
#define PI_SERVER_POWER_OFF 0

class PiServer {
    public:
        PiServer();

        void init();
        void tick();

        void set_power(int onOff);
        void set_visualizer(int type);

        int powered_on();
        int visualizer();

    private:
        int power_status;
        int visualizer_type;
};

#endif
