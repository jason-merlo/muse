/* ================================================================== *
 *  Written by Steven Ploog
 *
 *  updates:
 *
 *  File: settings.h
 *
 *  Description: Holds settings that vary person to person/by hardware instance.
 * ================================================================== */

#ifndef MUSE_SETTINGS_H
#define MUSE_SETTINGS_H

#include "muse.h"

// Enable/disable major systems
#define ENABLE_BARS           0
#define ENABLE_MSGEQ7         0
#define ENABLE_PSU_CONTROL    0
#define ENABLE_RGB_SR         0
#define ENABLE_SCREENSAVER    0
#define ENABLE_AUTO_SHUTDOWN  0
#define ENABLE_WEB_SERVER     0
#define ENABLE_WEB_POWER      0
#define ENABLE_SERIAL         0
#define ENABLE_MDNS           0
#define ENABLE_PI_SERVER      0

// Visaulizer to display if no pi control
#define STATIC_VISUALIZER     VISUALIZER_BARS

// Screensaver constants
#define SCREENSAVER_MINIMUM         1300
#define SCREENSAVER_SECS_TO_PSU_OFF 30
#define SCREENSAVER_SECS_TO_START   10

// EQ bins
#define LEFT_63     0
#define LEFT_160    1
#define LEFT_400    2
#define LEFT_1000   3
#define LEFT_2500   4
#define LEFT_6250   5
#define LEFT_16000  6

#define RIGHT_63    0
#define RIGHT_160   1
#define RIGHT_400   2
#define RIGHT_1000  3
#define RIGHT_2500  4
#define RIGHT_6250  5
#define RIGHT_16000 6

#endif
