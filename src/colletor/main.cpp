#include "ble_server.h"

#ifdef DEVICE_COLLECTOR

void setup() {
  Serial.begin(115200);
  Serial.println("初始化采集器");

  // 初始化 BLE 服务器
  BLES::setup();
}

void loop() {}

#endif