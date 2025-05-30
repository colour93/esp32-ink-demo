#include "ble_client.h"
#include "connect.h"
#include "epd.h"
#include "mqtt.h"
#include "pages/data_page.h"
#include "pages/page_manager.h"
#include "store.h"
#include "time_manager.h"
#ifdef DEVICE_DISPLAY

unsigned long lastReportTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("初始化");

  // 初始化 EPD
  EPD::setup();

  // 初始化页面
  Pages::setup();

  // 初始化并连接 WiFi
  Connect::setup();

  // 初始化时间
  if (Connect::isConnected()) {
    TimeManager::setup();
  }

  // 初始化 MQTT
  MQTT::setup();

  // 初始化 BLE 客户端
  BLEC::onSensorData([](const String &data) {
    // 解析传感器数据
    float temperature, humidity, ppm, pressure, altitude;
    if (data.length() >= sizeof(float) * 5) {
      memcpy(&temperature, data.c_str(), sizeof(float));
      memcpy(&humidity, data.c_str() + sizeof(float), sizeof(float));
      memcpy(&ppm, data.c_str() + sizeof(float) * 2, sizeof(float));
      memcpy(&pressure, data.c_str() + sizeof(float) * 3, sizeof(float));
      memcpy(&altitude, data.c_str() + sizeof(float) * 4, sizeof(float));

      // 缓存数据
      Store::temperature = temperature;
      Store::humidity = humidity;
      Store::ppm = ppm;
      Store::pressure = pressure;
      Store::altitude = altitude;

      Serial.printf("解析后的数据: 温度=%.2f, 湿度=%.2f, PPM=%.2f, 气压=%.2f, 海拔=%.2f\n",
                    temperature, humidity, ppm, pressure, altitude);

      // 距离上次上报大于 30 秒
      if (millis() - lastReportTime > 30000) {
        MQTT::publishSensorData();
        lastReportTime = millis();
      }
    }
  });

  // 添加触发器数据的回调
  BLEC::onTriggerData([](const String &data) {
    // 处理触发器数据
    if (data == "BUTTON_PRESSED") {
      Pages::switchToNextPage();
    }
  });

  BLEC::setup();

  // 初始化结束
  Serial.println("初始化结束");
}

void loop() {

  delay(1);

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
    if (!TimeManager::isTimeValid() && Connect::isConnected()) {
      TimeManager::setup();
    }
  }

  // 每 3000ms 执行一次
  if (currentTime % 3000 == 0) {
    // 更新显示
    if (Pages::currentPage == Pages::PageType::DATA) {
      Pages::DataPage::display();
    }
    if (Pages::currentPage == Pages::PageType::WELCOME) {
      Pages::WelcomePage::display();
    }
  }
}

#endif