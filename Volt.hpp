// Volt.hpp : Include file for the emulator
// Exposes everythi

#pragma once


// The different engine subsystems
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdarg>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <common/common.hpp>        // Common between client and server incl. network system
#include <emulation/emulation.hpp>  // Emulation
#include <sys/sys.hpp>              // System Specific Layer
#include <util/util.hpp>            // Utilities


// Global Defines
#define APP_NAME			"Volt Research Emulator"
#define VERSION		        "Pre-Alpha 1.0"
#define VERSION_STRING		"Version " VERSION
#define BUILD_DATE          __DATE__ " " __TIME__
#define	BUILD_STRING 		"Built " BUILD_STRING

#define ARRAY_SIZE(x)		sizeof(x)/sizeof(x[0])

#define M_PI_D              3.14159265358979323846      // Pi, double-precision

