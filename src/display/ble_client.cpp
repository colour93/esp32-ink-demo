#include "ble_client.h"

namespace BLEC {

static BLEUUID serviceUUID(SERVICE_UUID);
static BLEUUID sensorCharUUID(SENSOR_CHAR_UUID);
static BLEUUID triggerCharUUID(TRIGGER_CHAR_UUID);
static BLERemoteCharacteristic *pSensorCharacteristic;
static BLERemoteCharacteristic *pTriggerCharacteristic;
static BLEAdvertisedDevice *myDevice;
static bool deviceConnected = false;
static BLEClient *pClient = nullptr;
static void (*sensorDataCallback)(const String &) = nullptr;
bool doConnect = false;

class ClientCallbacks : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) override {
    deviceConnected = true;
    Serial.println("已连接到采集器");
  }

  void onDisconnect(BLEClient *pclient, int reason) override {
    deviceConnected = false;
    Serial.println("与采集器断开连接");
    EPD::displayCollectorStatus(1);
  }

  void onConnectFail(BLEClient *pclient, int reason) override {
    Serial.printf("连接失败,错误码: %d\n", reason);
  }

} clientCallbacks;

void notifyCallback(BLERemoteCharacteristic *pChar, uint8_t *pData,
                    size_t length, bool isNotify) {
  String data = String((char *)pData, length);
  if (sensorDataCallback) {
    sensorDataCallback(data);
  }
}

class ScanCallbacks : public NimBLEScanCallbacks {
  void onDiscovered(const BLEAdvertisedDevice *advertisedDevice) override {
    Serial.print("发现设备: ");
    Serial.println(advertisedDevice->toString().c_str());

    if (advertisedDevice->isAdvertisingService(NimBLEUUID(SERVICE_UUID))) {

      Serial.println("通过 UUID 找到采集器！");
      NimBLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(*advertisedDevice);
      doConnect = true;
    }
  }
} scanCallbacks;

bool connectToServer() {
  if (!myDevice) {
    Serial.println("没有可用的设备");
    return false;
  }

  if (pClient != nullptr) {
    if (pClient->isConnected()) {
      pClient->disconnect();
      delay(100);
    }
    delay(100);
    pClient = nullptr;
  }

  if (NimBLEDevice::getCreatedClientCount()) {
    pClient = NimBLEDevice::getClientByPeerAddress(myDevice->getAddress());
    if (pClient) {
      if (!pClient->connect(myDevice, false)) {
        Serial.printf("重连失败\n");
        return false;
      }
      Serial.printf("客户端已重连\n");
    } else {
      pClient = NimBLEDevice::getDisconnectedClient();
    }
  }

  if (!pClient) {
    if (NimBLEDevice::getCreatedClientCount() >= NIMBLE_MAX_CONNECTIONS) {
      Serial.printf("达到最大客户端数 - 没有更多可用连接\n");
      return false;
    }

    pClient = NimBLEDevice::createClient();

    Serial.printf("已创建新客户端\n");

    pClient->setClientCallbacks(&clientCallbacks, false);

    if (!pClient->connect(myDevice)) {
      NimBLEDevice::deleteClient(pClient);
      Serial.printf("连接失败,已删除客户端\n");
      return false;
    }
  }

  if (!pClient->isConnected()) {
    if (!pClient->connect(myDevice)) {
      Serial.printf("连接失败\n");
      return false;
    }
  }

  Serial.printf("已连接到: %s RSSI: %d\n",
                pClient->getPeerAddress().toString().c_str(),
                pClient->getRssi());

  Serial.println("正在获取服务...");
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.println("无法找到服务 UUID: " + String(SERVICE_UUID));
    pClient->disconnect();
    return false;
  }

  Serial.println("正在获取特征...");
  pSensorCharacteristic = pRemoteService->getCharacteristic(sensorCharUUID);
  if (pSensorCharacteristic == nullptr) {
    Serial.println("无法找到传感器特征 UUID: " + String(SENSOR_CHAR_UUID));
    pClient->disconnect();
    return false;
  }

  pTriggerCharacteristic = pRemoteService->getCharacteristic(triggerCharUUID);
  if (pTriggerCharacteristic == nullptr) {
    Serial.println("无法找到触发器特征 UUID: " + String(TRIGGER_CHAR_UUID));
    pClient->disconnect();
    return false;
  }

  if (pSensorCharacteristic->canNotify()) {
    Serial.println("注册通知回调...");
    pSensorCharacteristic->subscribe(true, notifyCallback);
  }

  EPD::displayCollectorStatus(1);

  return true;
}

void setup() {
  Serial.println("初始化 BLE 客户端");
  BLEDevice::init(CLIENT_BLE_NAME);

  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setScanCallbacks(&scanCallbacks, false);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(100);
  pBLEScan->setActiveScan(true);

  Serial.println("开始首次扫描...");
  pBLEScan->start(5000);
  Serial.println("首次扫描完成");
}

bool isConnected() { return deviceConnected; }

void sendTrigger(const String &data) {
  if (deviceConnected && pTriggerCharacteristic) {
    pTriggerCharacteristic->writeValue(data.c_str(), data.length());
  }
}

void onSensorData(void (*callback)(const String &)) {
  sensorDataCallback = callback;
}

} // namespace BLEC