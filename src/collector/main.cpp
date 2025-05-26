#ifdef DEVICE_COLLECTOR

#include "ble_server.h"
#include "dht11.h"
#include "mq135.h"

#define DHT11_PIN 22
#define MQ135_A0 34
#define BUTTON_PIN 23
#define DEBOUNCE_TIME 50

volatile bool buttonPressed = false;
volatile unsigned long lastDebounceTime = 0;
volatile unsigned long lastDataTime = 0;

void IRAM_ATTR buttonISR() {
  unsigned long currentTime = millis();
  if ((currentTime - lastDebounceTime) > DEBOUNCE_TIME) {
    buttonPressed = true;
    lastDebounceTime = currentTime;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("初始化采集器");

  // 初始化按钮
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(BUTTON_PIN, buttonISR, FALLING);

  // 初始化 BLE 服务器
  BLES::setup();

  // 初始化 MQ135
  MQ135::setup(MQ135_A0);

  // 初始化 DHT11
  DHTSensor::setup(DHT11_PIN);

  // 初始化结束
  Serial.println("初始化结束");

  // 获取 MQ135
  Serial.println("MQ135: " + String(MQ135::getPPM()));
  Serial.println("DHT11: " + String(DHTSensor::getTemperature()) + "C, " +
                 String(DHTSensor::getHumidity()) + "%");
}

void loop() {
  if (buttonPressed) {
    Serial.println("按钮被按下！");
    // 发送触发信号到显示器
    BLES::sendTriggerData("BUTTON_PRESSED");
    buttonPressed = false;
  }

  // 每 1000ms 获取一次 MQ135 和 DHT11 数据，notify
  if (millis() - lastDataTime >= 1000) {
    lastDataTime = millis();
    BLES::sendSensorData(DHTSensor::getTemperature(), DHTSensor::getHumidity(),
                         MQ135::getPPM());
  }
}

#endif