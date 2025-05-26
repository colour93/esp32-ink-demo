#ifndef MQ135_H
#define MQ135_H

#include <Arduino.h>

namespace MQ135 {

float getPPM();
void setup(uint8_t pin);

} // namespace MQ135

#endif