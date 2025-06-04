#include "oled.h"

#define SDA_PIN 19
#define SCL_PIN 18

namespace OLED {

// 使用软件I2C，这样可以指定引脚
U8G2_SSD1306_128X64_NONAME_F_SW_I2C *display = nullptr;

// 动画效果变量
uint8_t currentRadius = 2;  // 当前半径
bool isGrowing = true;      // 是否在增大
const uint8_t MIN_RADIUS = 1;   // 最小半径
const uint8_t MAX_RADIUS = 8;   // 最大半径

void setup() {
  display = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, SCL_PIN, SDA_PIN, U8X8_PIN_NONE);
  display->begin();
  display->enableUTF8Print();  // 启用UTF8支持
  display->setFont(u8g2_font_wqy12_t_gb2312);  // 设置中文字体
  display->setFontDirection(0);
}

void displayAnimation() {
  // 保留空函数以保持兼容性
}

// 更新动画效果
void updateCircleAnimation() {
  if (isGrowing) {
    currentRadius++;
    if (currentRadius >= MAX_RADIUS) {
      isGrowing = false;
    }
  } else {
    currentRadius--;
    if (currentRadius <= MIN_RADIUS) {
      isGrowing = true;
    }
  }
}

void displaySensorData(float temperature, float humidity, float ppm,
                      float pressure, float altitude, bool buttonState,
                      bool displayConnected) {
  display->clearBuffer();
  
  display->setCursor(0, 12);
  display->print(String("温度: ") + String(temperature) + String("°C"));
  
  display->setCursor(0, 24);
  display->print(String("湿度: ") + String(humidity) + String("%"));
  
  display->setCursor(0, 36);
  display->print(String("PPM: ") + String(ppm) + String("ppm"));
  
  display->setCursor(0, 48);
  display->print(String("气压: ") + String(pressure) + String("hPa"));
  
  display->setCursor(0, 60);
  display->print(String("海拔: ") + String(altitude) + String("m"));
  
  // 右侧状态
  display->setCursor(90, 12);
  display->print(displayConnected ? "已连接" : "未连接");
  
  // 更新并绘制动画效果
  updateCircleAnimation();
  
  // 在右下角绘制圆形
  uint8_t x = 110;     // 圆心x坐标
  uint8_t y = 50;      // 圆心y坐标
  
  // 绘制实心圆
  display->drawDisc(x, y, currentRadius);
  
  display->sendBuffer();
}

} // namespace OLED