#pragma once

#include "time.h"
#include <Arduino.h>

namespace TimeManager {

extern struct tm timeinfo;

void setup();
void updateTime();
bool isTimeValid();
bool getTimeInfo(struct tm *result);
} // namespace TimeManager
