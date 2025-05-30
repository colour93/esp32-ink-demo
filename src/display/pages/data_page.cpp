#include "data_page.h"

namespace Pages
{

  // 缓存上一次显示的数据
  static float lastTemperature = -1;
  static float lastHumidity = -1;
  static float lastPpm = -1;
  static float lastPressure = -1;
  static float lastAltitude = -1;

  void DataPage::display(bool forceRefresh)
  {
    Serial.println("显示数据页");

    // 如果是强制刷新,重新绘制所有内容
    if (forceRefresh)
    {

      EPD::u8g2.setFont(u8g2_font_wqy16_t_gb2312);

      EPD::u8g2.setCursor(DATA_PAGE_HUMIDITY_X, DATA_PAGE_HUMIDITY_Y);
      EPD::u8g2.print("湿度");

      EPD::u8g2.setCursor(DATA_PAGE_TEMPERATURE_X, DATA_PAGE_TEMPERATURE_Y);
      EPD::u8g2.print("温度");

      EPD::u8g2.setCursor(DATA_PAGE_PPM_X, DATA_PAGE_PPM_Y);
      EPD::u8g2.print("空气");

      EPD::u8g2.setCursor(DATA_PAGE_ALTITUDE_X, DATA_PAGE_ALTITUDE_Y);
      EPD::u8g2.print("海拔");

      EPD::u8g2.setCursor(DATA_PAGE_PRESSURE_X, DATA_PAGE_PRESSURE_Y);
      EPD::u8g2.print("气压");

      lastTemperature = -1;
      lastHumidity = -1;
      lastPpm = -1;
      lastPressure = -1;
      lastAltitude = -1;
    }

    EPD::u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    bool needRefresh = false;

    // 检查湿度是否变化
    if (Store::humidity != lastHumidity)
    {
      int y = DATA_PAGE_HUMIDITY_Y + DATA_PAGE_COMPONENT_HEIGHT +
              DATA_PAGE_COMPONENT_LINE_GAP;
      EPD::display.fillRect(DATA_PAGE_HUMIDITY_X, y - 16,
                            DATA_PAGE_COMPONENT_WIDTH, 20, GxEPD_WHITE);
      EPD::u8g2.setCursor(DATA_PAGE_HUMIDITY_X, y);
      EPD::u8g2.print(String(Store::humidity) + "%");
      lastHumidity = Store::humidity;
      needRefresh = true;
    }

    // 检查温度是否变化
    if (Store::temperature != lastTemperature)
    {
      int y = DATA_PAGE_TEMPERATURE_Y + DATA_PAGE_COMPONENT_HEIGHT +
              DATA_PAGE_COMPONENT_LINE_GAP;
      EPD::display.fillRect(DATA_PAGE_TEMPERATURE_X, y - 16,
                            DATA_PAGE_COMPONENT_WIDTH, 20, GxEPD_WHITE);
      EPD::u8g2.setCursor(DATA_PAGE_TEMPERATURE_X, y);
      EPD::u8g2.print(String(Store::temperature) + "C");
      lastTemperature = Store::temperature;
      needRefresh = true;
    }

    // 检查PPM是否变化
    if (Store::ppm != lastPpm)
    {
      int y = DATA_PAGE_PPM_Y + DATA_PAGE_COMPONENT_HEIGHT +
              DATA_PAGE_COMPONENT_LINE_GAP;
      EPD::display.fillRect(DATA_PAGE_PPM_X, y - 16, DATA_PAGE_COMPONENT_WIDTH,
                            20, GxEPD_WHITE);
      EPD::u8g2.setCursor(DATA_PAGE_PPM_X, y);
      EPD::u8g2.print(String(Store::ppm));
      lastPpm = Store::ppm;
      needRefresh = true;
    }

    // 检查海拔是否变化
    if (Store::altitude != lastAltitude)
    {
      int y = DATA_PAGE_ALTITUDE_Y + DATA_PAGE_COMPONENT_HEIGHT +
              DATA_PAGE_COMPONENT_LINE_GAP;
      EPD::display.fillRect(DATA_PAGE_ALTITUDE_X, y - 16, DATA_PAGE_COMPONENT_WIDTH,
                            20, GxEPD_WHITE);
      EPD::u8g2.setCursor(DATA_PAGE_ALTITUDE_X, y);
      EPD::u8g2.print(String(Store::altitude));
      lastAltitude = Store::altitude;
      needRefresh = true;
    }

    // 检查气压是否变化
    if (Store::pressure != lastPressure)
    {
      int y = DATA_PAGE_PRESSURE_Y + DATA_PAGE_COMPONENT_HEIGHT +
              DATA_PAGE_COMPONENT_LINE_GAP;
      EPD::display.fillRect(DATA_PAGE_PRESSURE_X, y - 16, DATA_PAGE_COMPONENT_WIDTH,
                            20, GxEPD_WHITE);
      EPD::u8g2.setCursor(DATA_PAGE_PRESSURE_X, y);
      EPD::u8g2.print(String(Store::pressure));
      lastPressure = Store::pressure;
      needRefresh = true;
    }

    // 只有在数据发生变化或强制刷新时才更新显示
    if (needRefresh || forceRefresh)
    {
      EPD::display.display(1);
    }
  }

} // namespace Pages