#ifndef CONNECT_H
#define CONNECT_H

#include "config.h"
#include "epd.h"
#include <WiFi.h>

namespace Connect {
void setup();
void connect();
bool isConnected();
void updateStatus();
} // namespace Connect

#endif
