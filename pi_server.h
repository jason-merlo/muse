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

#define PI_MSG_LEN          3
#define PI_MAX_MSG_LEN      3

#define PI_MSG_START        0xAB
#define PI_MSG_END          0xCD

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
        bool get_msg();
        bool is_valid_msg();

        UDP udp;

        unsigned char message[PI_MAX_MSG_LEN];

        int power_status;
        int visualizer_type;
        int num_msgs;
        int last_msg;

        bool pi_clock_went_low;
};

#endif
