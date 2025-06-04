#ifdef DEVICE_COLLECTOR

#include "ble_server.h"
#include "bmp180.h"
#include "dht11.h"
#include "mq135.h"
#include "oled.h"

// 传感器引脚定义
#define DHT11_PIN 32
#define MQ135_A0 34
#define BUTTON_PIN 5

// BMP180 软 I2C 引脚定义
#define BMP180_SDA 14
#define BMP180_SCL 27

#define DEBOUNCE_TIME 200

volatile bool buttonPressed = false;
volatile unsigned long lastDebounceTime = 0;
volatile unsigned long lastDataTime = 0;
volatile unsigned long lastOLEDUpdateTime = 0;

void IRAM_ATTR buttonISR() {
  unsigned long currentTime = millis();
  if ((currentTime - lastDebounceTime) > DEBOUNCE_TIME) {
    buttonPressed = true;
    lastDebounceTime = currentTime;
  }
}

float temperature = 0;
float humidity = 0;
float ppm = 0;
float pressure = 0;
float altitude = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("初始化采集器");

  OLED::setup();

  // 初始化按钮
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(BUTTON_PIN, buttonISR, FALLING);

  // 初始化 BLE 服务器
  BLES::setup();

  // 初始化 MQ135
  MQ135::setup(MQ135_A0);

  // 初始化 DHT11
  DHTSensor::setup(DHT11_PIN);

  // 初始化 BMP180 软 I2C
  BMP180::setupSoftI2C(BMP180_SDA, BMP180_SCL);
  BMP180::setup();

  // 初始化结束
  Serial.println("初始化结束");

  // 获取传感器数据
  Serial.println("MQ135: " + String(MQ135::getPPM()));
  Serial.println("DHT11: " + String(DHTSensor::getTemperature()) + "C, " +
                 String(DHTSensor::getHumidity()) + "%");
  Serial.println("BMP180: " + String(BMP180::getTemperature()) + "C, " +
                 String(BMP180::getPressure()) + "hPa, " +
                 String(BMP180::getAltitude()) + "m");
}

void loop() {
  if (buttonPressed) {
    Serial.println("按钮被按下！");
    // 发送触发信号到显示器
    BLES::sendTriggerData("BUTTON_PRESSED");
    buttonPressed = false;
  }

  // 每 1000ms 获取一次传感器数据，notify
  if (millis() - lastDataTime >= 1000) {
    lastDataTime = millis();

    // 发送数据到BLE
    BLES::sendSensorData(temperature, humidity, ppm, pressure, altitude);
  }

  // 每 50ms 更新一次 OLED 显示
  if (millis() - lastOLEDUpdateTime >= 10) {
    lastOLEDUpdateTime = millis();

    // 获取所有传感器数据
    temperature = DHTSensor::getTemperature();
    humidity = DHTSensor::getHumidity();
    ppm = MQ135::getPPM();
    pressure = BMP180::getPressure();
    altitude = BMP180::getAltitude();
    OLED::displaySensorData(temperature, humidity, ppm, pressure, altitude,
                            buttonPressed, BLES::isConnected());
  }
}

#endif