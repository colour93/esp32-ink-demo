#include "dht11.h"

namespace DHTSensor {

DHT dht(0, DHT11, 6);

void setup(uint8_t pin) {
  pinMode(pin, INPUT);
  dht = DHT(pin, DHT11, 6);
  dht.begin();
}

float getTemperature() { return dht.readTemperature(); }

float getHumidity() { return dht.readHumidity(); }

} // namespace DHTSensor
