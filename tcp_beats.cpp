#include "tcp_beats.h"

#include <stdio.h>

const unsigned char server[] = {10, 0, 0, 70}; // Cabinet light RPi

TCPBeats::TCPBeats() {
    tcp.connect(server, 80);
    tcp.flush();
}

void TCPBeats::stevenSendRGB(int r, int g, int b) {
    if (tcp.connected()) {
        tcp.flush();

        char nums [20];
        sprintf(nums, "&r=%03d&g=%03d&b=%03d\r\n", r, g, b);

        tcp.print("POST /rgb/web_input/ HTTP/1.1\r\nHost: rgb.local\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 26\r\n\r\ntype=rgb");
        tcp.print(nums);

        // tcp.print(r, DEC);
        // tcp.print("&g=");
        // tcp.print(g, DEC);
        // tcp.print("&b=");
        // tcp.print(b, DEC);
        // tcp.print("\r\n");
    }

}

//"POST /rgb/web_input/ HTTP/1.1\r\nHost: rgb.local\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 22\r\n\r\ntype=rgb&rgb=#"


// POST /rgb/web_input/ HTTP/1.1
// Host: rgb.local
// Connection: keep-alive
// Content-Length: 22
// Origin: http://rgb.local
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36
// Content-Type: application/x-www-form-urlencoded
// Accept: */*
// X-Requested-With: XMLHttpRequest
// X-CSRFToken: undefined
// Referer: http://rgb.local/rgb/
// Accept-Encoding: gzip, deflate
// Accept-Language: en-US,en;q=0.8,de;q=0.6
// type=rgb&rgb=%230000ff



// curl 'http://rgb.local/rgb/web_input/' \
// -XPOST \
// -H 'Content-Type: application/x-www-form-urlencoded' \
// -H 'Referer: http://rgb.local/rgb/' \
// -H 'Accept: */*' \
// -H 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_1) AppleWebKit/602.2.14 (KHTML, like Gecko) Version/10.0.1 Safari/602.2.14' \
// -H 'Origin: http://rgb.local' \
// -H 'X-CSRFToken: dNJrBux676373d00u3wVroXHD6b9scF6' \
// -H 'X-Requested-With: XMLHttpRequest' \
// --data 'type=rgb&rgb=%230000FF'


// POST /rgb/web_input/ HTTP/1.1
// Host: rgb.local
// Connection: keep-alive
// Content-Length: 52
// Origin: http://rgb.local
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36
// Content-Type: application/x-www-form-urlencoded
// Accept: */*
// X-Requested-With: XMLHttpRequest
// X-CSRFToken: undefined
// Referer: http://rgb.local/rgb/
// Accept-Encoding: gzip, deflate
// Accept-Language: en-US,en;q=0.8,de;q=0.6
