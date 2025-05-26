#ifndef DHT11_H
#define DHT11_H

#include <DHT.h>

// 改用不同的命名空间名称,避免与DHT库冲突
namespace DHTSensor {

void setup(uint8_t pin);

float getTemperature();

float getHumidity();

} // namespace DHTSensor

#endif
