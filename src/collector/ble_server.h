#ifndef BLE_SERVER_H
#define BLE_SERVER_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>

#include "config.h"

namespace BLES {

// BLE 服务器和特征值指针
extern NimBLEServer* pServer;
extern NimBLECharacteristic* pSensorCharacteristic;
extern NimBLECharacteristic* pTriggerCharacteristic;
extern bool deviceConnected;

void setup();
void sendSensorData(float temperature, float humidity, float ppm);
void sendTriggerData(const String &data);
bool isConnected();

} // namespace BLES

#endif