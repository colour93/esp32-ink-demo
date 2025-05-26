#ifdef DEVICE_COLLECTOR

#include "ble_server.h"
#include "mq135.h"

#define MQ135_A0 34
#define BUTTON_PIN 23
#define DEBOUNCE_TIME 50 // 消抖时间(毫秒)

volatile bool buttonPressed = false;
volatile unsigned long lastDebounceTime = 0;

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

  // 初始化结束
  Serial.println("初始化结束");

  // 获取 MQ135
  Serial.println("数据: " + String(MQ135::getPPM()));
}

void loop() {
  if(buttonPressed) {
    Serial.println("按钮被按下!");
    // 发送触发信号到显示器
    BLES::sendTriggerData("BUTTON_PRESSED");
    buttonPressed = false;
  }
  
  // 其他主循环任务可以在这里添加
}

#endif