#ifndef BLE_CLIENT_H
#define BLE_CLIENT_H

#include "config.h"
#include "epd.h"
#include <NimBLEDevice.h>

namespace BLEC {

extern bool doConnect;

// 公共接口
void setup();
bool isConnected();
void sendTrigger(const String &data);
void onSensorData(void (*callback)(const String &));

// 内部函数声明
bool connectToServer();
void notifyCallback(BLERemoteCharacteristic *pChar, uint8_t *pData,
                    size_t length, bool isNotify);

} // namespace BLEC

#endif