#ifndef TCP_BEATS_H_
#define TCP_BEATS_H_

#include "application.h"

class TCPBeats {
    public:
        TCPBeats();
        void init();
        void stevenSendRGB(int r, int g, int b);

    private:
        UDP udp;
        String str;

        unsigned char cmd_buffer[25];
        int udpsends;
        int udpcode;
};

#endif
