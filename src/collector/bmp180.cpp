#include "bmp180.h"

namespace BMP180 {

Adafruit_BMP085 bmp;
TwoWire *softWire = nullptr;

void setupSoftI2C(uint8_t sda, uint8_t scl) {
  if (softWire == nullptr) {
    softWire = new TwoWire(1); // 使用 Wire1
    softWire->begin(sda, scl);
  }
}

void setup() {
  if (!bmp.begin(0x77, softWire)) {  // 使用软 I2C 接口
    Serial.println("无法找到 BMP180 传感器!");
    while (1) {}
  }
  Serial.println("BMP180 初始化成功");
}

float getPressure() {
  // 获取气压值，单位：hPa (百帕)
  return bmp.readPressure() / 100.0F;
}

float getTemperature() {
  // 获取温度值，单位：°C
  return bmp.readTemperature();
}

float getAltitude() {
  // 获取海拔高度，单位：米
  // 使用标准大气压 1013.25 hPa 作为参考
  return bmp.readAltitude();
}

} // namespace BMP180 