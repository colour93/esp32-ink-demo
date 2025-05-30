#ifndef BMP180_H
#define BMP180_H

#include <Adafruit_BMP085.h>
#include <Wire.h>

namespace BMP180 {

void setup();
void setupSoftI2C(uint8_t sda, uint8_t scl);  // 添加软 I2C 设置函数
float getPressure();    // 获取气压值，单位：hPa
float getTemperature(); // 获取温度值，单位：°C
float getAltitude();    // 获取海拔高度，单位：米

} // namespace BMP180

#endif // BMP180_H 