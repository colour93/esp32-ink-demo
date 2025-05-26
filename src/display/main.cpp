#include "ble_client.h"
#include "connect.h"
#include "epd.h"
#include "pages/page_manager.h"

#ifdef DEVICE_DISPLAY

void setup() {
  Serial.begin(115200);
  Serial.println("初始化");

  // 初始化 EPD
  EPD::setup();

  // 初始化页面
  Pages::setup();

  // 初始化并连接 WiFi
  Connect::setup();

  // 初始化 BLE 客户端
  BLEC::setup();
  BLEC::onSensorData([](const String &data) {
    Serial.println("收到传感器数据: " + data);
    // 这里可以处理接收到的数据
  });

  // 初始化结束
  Serial.println("初始化结束");
}

void loop() {

  const unsigned long currentTime = millis();

  // 每 100ms 执行一次
  if (currentTime % 100 == 0) {

    if (BLEC::doConnect) {
      BLEC::doConnect = false;
      if (BLEC::connectToServer()) {
        Serial.println("连接成功！");
      } else {
        Serial.println("连接失败！");
      }
    }
  }

  // 每 10s 执行一次
  if (currentTime % 10000 == 0) {
    if (!BLEC::isConnected()) {
      NimBLEDevice::getScan()->start(5000, false, true);
    }
  }
}

#endif