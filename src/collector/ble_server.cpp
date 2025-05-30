#include "ble_server.h"
namespace BLES {

NimBLEServer *pServer = nullptr;
NimBLECharacteristic *pSensorCharacteristic = nullptr;
NimBLECharacteristic *pTriggerCharacteristic = nullptr;
bool deviceConnected = false;

class SensorCallbacks : public NimBLECharacteristicCallbacks {
  void onRead(NimBLECharacteristic *pCharacteristic,
              NimBLEConnInfo &connInfo) override {
    Serial.printf("%s : onRead(), 值: %s\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
  }

  void onWrite(NimBLECharacteristic *pCharacteristic,
               NimBLEConnInfo &connInfo) override {
    Serial.printf("%s : onWrite(), 值: %s\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
  }

  void onStatus(NimBLECharacteristic *pCharacteristic, int code) override {
    Serial.printf("通知/指示返回代码: %d, %s\n", code,
                  NimBLEUtils::returnCodeToString(code));
  }

  void onSubscribe(NimBLECharacteristic *pCharacteristic,
                   NimBLEConnInfo &connInfo, uint16_t subValue) override {
    std::string str = "客户端ID: ";
    str += connInfo.getConnHandle();
    str += " 地址: ";
    str += connInfo.getAddress().toString();
    if (subValue == 0) {
      str += " 取消订阅 ";
    } else if (subValue == 1) {
      str += " 订阅通知 ";
    } else if (subValue == 2) {
      str += " 订阅指示 ";
    } else if (subValue == 3) {
      str += " 订阅通知和指示 ";
    }
    str += std::string(pCharacteristic->getUUID());

    Serial.printf("%s\n", str.c_str());
  }
} sensorCallbacks;

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) {
    deviceConnected = true;
    Serial.println("设备已连接");
    Serial.printf("客户端地址: %s\n", connInfo.getAddress().toString().c_str());
    pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
  }

  void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo,
                    int reason) {
    deviceConnected = false;
    Serial.println("设备已断开");
    pServer->startAdvertising();
  }

} serverCallbacks;

void setup() {
  Serial.begin(115200);
  Serial.println("启动 BLE 服务器");

  NimBLEDevice::init(SERVER_BLE_NAME);

  NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_SC);

  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(&serverCallbacks);

  NimBLEService *pService = pServer->createService(SERVICE_UUID);

  pSensorCharacteristic = pService->createCharacteristic(
      SENSOR_CHAR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  pSensorCharacteristic->setCallbacks(&sensorCallbacks);

  pTriggerCharacteristic = pService->createCharacteristic(
      TRIGGER_CHAR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  pTriggerCharacteristic->setCallbacks(&sensorCallbacks);

  pService->start();

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setName(SERVER_BLE_NAME);
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->enableScanResponse(true);
  pAdvertising->start();

  Serial.println("BLE 服务器已启动");
  Serial.println("Mac 地址: " +
                 String(NimBLEDevice::getAddress().toString().c_str()));
}

void sendSensorData(float temperature, float humidity, float ppm, float pressure, float altitude) {
  if (deviceConnected) {
    float values[5] = {temperature, humidity, ppm, pressure, altitude};

    pSensorCharacteristic->setValue((uint8_t *)values, sizeof(values));
    pSensorCharacteristic->notify();

    Serial.printf("发送传感器数据: 温度=%.2f, 湿度=%.2f, PPM=%.2f, 气压=%.2f, 海拔=%.2f\n",
                  temperature, humidity, ppm, pressure, altitude);
  }
}

void sendTriggerData(const String &data) {
  if (deviceConnected) {
    pTriggerCharacteristic->setValue(data.c_str());
    pTriggerCharacteristic->notify();
    Serial.println("发送触发器数据: " + data);
  }
}

bool isConnected() { return deviceConnected; }

} // namespace BLES