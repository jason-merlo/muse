#ifndef TCP_BEATS_H_
#define TCP_BEATS_H_

#include "application.h"

class TCPBeats {
    public:
        TCPBeats();
        void stevenSendRGB(int r, int g, int b);

    private:
        TCPClient tcp;
};

#endif
