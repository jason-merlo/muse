/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  File: muse.cpp
 *
 *  Description: Defines visualizer webpage content
 * ================================================================== */

#ifndef WEBPAGE_CONSTANTS_H
#define WEBPAGE_CONSTANTS_H

#include "application.h"

// Webpage content
static const char webpage[] =
"<HTML>"
"<head>"
"<style type=\"text/css\">"
"body {"
""
"}"
"</style>"
"<head>"
"<body>"
"<h1>Spark Webpage!</h1>"
"</body>"
"</HTML>";

static const char http_ok[] = "HTTP/1.1 200 OK";
static const char http_content_type[] = "Content-Type: text/html";
static const char http_content_length[] = "Content-Length: ";
static const char http_connection_close[] = "Connection: close";

String webpage_string1=
"<HTML>\
<head>\
<style type=\"text/css\">\
body {\
\
}\
</style>\
<head>\
<body>\
<h1>Spark Webpage!</h1>\
</body>\
</HTML>";

const char index_html[] = "<!DOCTYPE html><html><head><title>Setup your device</title>/head><body><h1>Connect me to your WiFi!</h1></body></html>";

#endif
