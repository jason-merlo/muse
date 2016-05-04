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

static const unsigned char hello_world[] = "<h1>Hello, World!</h1>";

static const unsigned char control_panel[] = "<!DOCTYPE html>"
"<html>"
    "<head>"
        "<title>MUSE Control Panel</title>"
        "<script>"
            "var currentValue = 0;"
            "function handleClick(myRadio) {"
                "currentValue = myRadio.value;"
                "document.getElementById('visualizer_form').submit();"
            "}"
        "</script>"
    "</head>"
    "<body>"
        "<form id='visualizer_form' action='/web_input' method='post'>"
            "<label for='v1'><input type='radio' id='v1' name='visualizer' onclick='handleClick(this)' value='0'></input>Bars</label><br>"
            "<label for='v2'><input type='radio' id='v2' name='visualizer' onclick='handleClick(this)' value='1'></input>Bars Middle</label><br>"
            "<label for='v3'><input type='radio' id='v3' name='visualizer' onclick='handleClick(this)' value='2'></input>Plasma</label><br>"
            "<label for='v4'><input type='radio' id='v4' name='visualizer' onclick='handleClick(this)' value='3'></input>Pulse</label><br>"
            "<label for='v5'><input type='radio' id='v5' name='visualizer' onclick='handleClick(this)' value='4'></input>Rainbow</label><br>"
            "<label for='v6'><input type='radio' id='v6' name='visualizer' onclick='handleClick(this)' value='5'></input>Wheel</label><br>"
        "</form>"
    "</body>"
"</html>";

static const unsigned char rgb_main_html[] =
    "<!DOCTYPE html>"
        "<html>"
            "<head>"
                "<title>Webduino AJAX RGB Example</title>"
                "<link href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/themes/base/jquery-ui.css' rel=stylesheet />"
                "<script src='http://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js'></script>"
                "<script src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/jquery-ui.min.js'></script>"
                "<style> body { background: black; } #red, #green, #blue { margin: 10px; } #red { background: #f00; } #green { background: #0f0; } #blue { background: #00f; } </style>"
                "<script>"
                // change color on mouse up, not while sliding (causes much less traffic to the Arduino):
                    "function changeRGB(event, ui) { var id = $(this).attr('id'); if (id == 'red') $.post('/rgb', { red: ui.value } ); if (id == 'green') $.post('/rgb', { green: ui.value } ); if (id == 'blue') $.post('/rgb', { blue: ui.value } ); } "
                    "$(document).ready(function(){ $('#red, #green, #blue').slider({min: 0, max:5, change:changeRGB}); });"

                // change color on slide and mouse up (causes more traffic to the Arduino):
                //"function changeRGB(event, ui) { jQuery.ajaxSetup({timeout: 110}); /*not to DDoS the Arduino, you might have to change this to some threshold value that fits your setup*/ var id = $(this).attr('id'); if (id == 'red') $.post('/rgb', { red: ui.value } ); if (id == 'green') $.post('/rgb', { green: ui.value } ); if (id == 'blue') $.post('/rgb', { blue: ui.value } ); } "
                //"$(document).ready(function(){ $('#red, #green, #blue').slider({min: 0, max:255, change:changeRGB, slide:changeRGB}); });"
                "</script>"
            "</head>"
        "<body style='font-size:62.5%;'>"
            "<div id=red></div>"
            "<div id=green></div>"
            "<div id=blue></div>"
        "</body>"
    "</html>";

#endif
