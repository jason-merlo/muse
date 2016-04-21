/* ================================================================== *
 *  Written by Jason Merlo
 *
 *  File: muse.cpp
 *
 *  Description: Defines visualizer webpage content
 * ================================================================== */

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

String webpage_string =
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
