#ifndef OLED_H
#define OLED_H

#include <U8g2lib.h>

namespace OLED {

void setup();
void displayAnimation();
void displaySensorData(float temperature, float humidity, float ppm,
                       float pressure, float altitude, bool buttonState,
                       bool displayConnected);

} // namespace OLED
#endif
