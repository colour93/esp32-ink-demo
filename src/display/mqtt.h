#ifndef MQTT_H
#define MQTT_H

#include "store.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <config.h>

namespace MQTT {

extern PubSubClient client;

void setup();
void publishSensorData();

} // namespace MQTT

#endif // MQTT_H
