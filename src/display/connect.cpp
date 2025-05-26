#include "connect.h"

namespace Connect {

void setup() {
  EPD::displayWifiStatus(false);
  WiFi.mode(WIFI_STA);
  connect();
}

void connect() {
  Serial.print("连接 WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // 等待连接，最多尝试20次
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("连接成功，IP 地址: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("连接失败");
  }

  updateStatus();
}

bool isConnected() { return WiFi.status() == WL_CONNECTED; }

void updateStatus() { EPD::displayWifiStatus(true); }

} // namespace Connect