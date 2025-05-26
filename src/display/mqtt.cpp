#include "mqtt.h"

namespace MQTT {

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length) {
  // 暂时不处理接收到的消息
}

void setup() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  Serial.println("MQTT 初始化");

  // 创建一个 FreeRTOS 任务来处理 MQTT 连接
  xTaskCreatePinnedToCore(mqttReconnectTask,   // 任务函数
                          "MQTTReconnectTask", // 任务名称
                          4096,                // 堆栈大小
                          NULL,                // 任务参数
                          1,                   // 优先级
                          NULL,                // 任务句柄
                          1                    // 运行在核心 1 上
  );
}

void mqttReconnectTask(void *parameter) {
  for (;;) {
    if (!client.connected()) {
      Serial.println("尝试 MQTT 连接...");
      String clientId = "ESP32-" + String(random(0xffff), HEX);

      if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
        Serial.println("MQTT 已连接");
        client.subscribe("attributes/get/response/+");
      } else {
        Serial.print("MQTT 连接失败, rc=");
        Serial.println(client.state());
      }
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS); // 每5秒尝试一次
  }
}

void publishSensorData() {
  if (!client.connected()) {
    // 如果未连接，等待任务重新连接
    return;
  }

  StaticJsonDocument<200> doc;
  doc["temperature"] = Store::temperature;
  doc["humidity"] = Store::humidity;
  doc["co2"] = Store::ppm;

  char jsonBuffer[200];
  serializeJson(doc, jsonBuffer);

  if (client.publish("attributes", jsonBuffer)) {
    Serial.println("已发送传感器数据到 MQTT");
  } else {
    Serial.println("MQTT 发送失败");
  }
}

void loop() {
  if (client.connected()) {
    client.loop();
  }
}

} // namespace MQTT
