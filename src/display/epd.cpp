#include "epd.h"

namespace EPD {

SPIClass hspi(HSPI);

GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT>
    display(GxEPD2_213_B74(/*CS=*/PIN_SPI_CS, /*DC=*/PIN_SPI_DC,
                           /*RST=*/PIN_SPI_RST, /*BUSY=*/PIN_SPI_BUSY));

U8G2_FOR_ADAFRUIT_GFX u8g2;

void setup() {
  // 初始化屏幕
  Serial.println("初始化屏幕");

  // 必须这么写
  hspi.begin(13, 12, 14, 15);
  display.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));

  display.init();
  display.setRotation(1);
  u8g2.begin(display);

  // 设置字体和颜色
  display.fillScreen(GxEPD_WHITE);
  u8g2.setFontMode(0);
  u8g2.setBackgroundColor(GxEPD_WHITE);
  u8g2.setForegroundColor(GxEPD_BLACK);

  display.firstPage();

  // 欢迎
  u8g2.setCursor(COMPONENT_TITLE_X, COMPONENT_TITLE_Y);
  u8g2.setFont(u8g2_font_helvB24_tr);
  u8g2.print("Starting...");
  u8g2.setCursor(COMPONENT_SUBTITLE_X, COMPONENT_SUBTITLE_Y);
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  u8g2.print("启启启启启动中...");

  displayStatus(0);

  // 显示
  display.display();
}

void clearScreen() {
  display.fillScreen(GxEPD_WHITE);
  display.display();
}

void clearCoreArea() {
  display.fillRect(CORE_DISPLAY_X, CORE_DISPLAY_Y, CORE_DISPLAY_WIDTH,
                   CORE_DISPLAY_HEIGHT, GxEPD_WHITE);
  display.display(1);
}

void displayStatus(const bool autoDisplay) {
  displayWifiStatus(false);
  displayCollectorStatus(false);
  if (autoDisplay)
    display.display(1);
}

void displayWifiStatus(const bool autoDisplay) {
  const unsigned char *pIcon;

  if (Connect::isConnected()) {
    pIcon = epd_bitmap_wifi_connect;
  } else {
    pIcon = epd_bitmap_wifi_disconnect;
  }

  display.fillRect(ICON_CONNECT_X, ICON_CONNECT_Y, ICON_STATUS_SIZE,
                   ICON_STATUS_SIZE, GxEPD_WHITE);
  display.drawInvertedBitmap(ICON_CONNECT_X, ICON_CONNECT_Y, pIcon,
                             ICON_STATUS_SIZE, ICON_STATUS_SIZE, GxEPD_BLACK);
  if (autoDisplay)
    display.display(1);
}

void displayCollectorStatus(const bool autoDisplay) {
  const unsigned char *pIcon;

  if (BLEC::isConnected()) {
    pIcon = epd_bitmap_collector_connect;
  } else {
    pIcon = epd_bitmap_collector_disconnect;
  }

  display.fillRect(ICON_COLLECTOR_X, ICON_COLLECTOR_Y, ICON_STATUS_SIZE,
                   ICON_STATUS_SIZE, GxEPD_WHITE);
  display.drawInvertedBitmap(ICON_COLLECTOR_X, ICON_COLLECTOR_Y, pIcon,
                             ICON_STATUS_SIZE, ICON_STATUS_SIZE, GxEPD_BLACK);
  if (autoDisplay)
    display.display(1);
}

} // namespace EPD