#include "mq135.h"

namespace MQ135 {

uint8_t PIN = 0;

// 校准参数
const float RLOAD = 10.0;      // RL = 10K Ω
const float RZERO = 76.63;     // R0 = 76.63 kΩ (在洁净空气中测得)
const float PARA = 116.6020682; // 校准参数
const float PARB = 2.769034857; // 校准参数

float getPPM() {
  // 读取模拟值
  int sensorValue = analogRead(PIN);
  
  // 将ADC值转换为电压值 (ESP32 ADC分辨率为12位, 即0-4095)
  float voltage = sensorValue * (3.3 / 4095.0);
  
  // 计算传感器电阻值 Rs
  float rs = ((3.3 * RLOAD) / voltage) - RLOAD;
  
  // 计算 rs/r0 比率
  float ratio = rs / RZERO;
  
  // 使用校准参数计算 PPM 值
  float ppm = PARA * pow((float)ratio, -PARB);
  
  return ppm;
}

void setup(uint8_t pin) {
  PIN = pin;
  // 设置ADC分辨率为12位(0-4095)
  analogReadResolution(12);
}

} // namespace MQ135

