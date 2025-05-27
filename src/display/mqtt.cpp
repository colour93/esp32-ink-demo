#include "mqtt.h"

namespace MQTT {

WiFiClient wifiClient;
PubSubClient client(wifiClient);
TaskHandle_t mqttTaskHandle = NULL;
SemaphoreHandle_t mqttMutex = NULL;  // 添加互斥锁

void callback(char *topic, byte *payload, unsigned int length) {
  // 暂时不处理接收到的消息
}

// 合并所有MQTT操作到一个任务中
void mqttTask(void *parameter) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(100); // 100ms 间隔
  
  for (;;) {
    if (xSemaphoreTake(mqttMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      // 检查连接
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
      } else {
        // 处理MQTT消息
        client.loop();
      }
      xSemaphoreGive(mqttMutex);
    }
    
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void setup() {
  mqttMutex = xSemaphoreCreateMutex();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  Serial.println("MQTT 初始化");

  // 创建MQTT任务
  xTaskCreatePinnedToCore(
    mqttTask,          // 任务函数
    "MQTTTask",        // 任务名称
    4096,              // 堆栈大小
    NULL,              // 任务参数
    1,                 // 优先级（低于TimeManager）
    &mqttTaskHandle,   // 任务句柄
    1                  // 运行在核心 1 上
  );
}

void publishSensorData() {
  if (xSemaphoreTake(mqttMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    if (client.connected()) {
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
    xSemaphoreGive(mqttMutex);
  }
}

} // namespace MQTT
