#include "welcome_page.h"

namespace Pages {

static int lastHour = -1;
static int lastMinute = -1;
static int lastSecond = -1;

void WelcomePage::display(bool forceRefresh) {
  Serial.println("显示时钟页面");

  if (!Connect::isConnected()) {
    EPD::clearCoreArea(0);

    EPD::u8g2.setFont(u8g2_font_helvB24_tr);
    EPD::u8g2.setCursor(COMPONENT_TITLE_X, COMPONENT_TITLE_Y);
    EPD::u8g2.print("Xixixixixixi");
    EPD::u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    EPD::u8g2.setCursor(COMPONENT_SUBTITLE_X, COMPONENT_SUBTITLE_Y);
    EPD::u8g2.print("嘻嘻嘻嘻嘻嘻");

    EPD::display.display(1);
    return;
  }

  struct tm currentTime;
  if (TimeManager::getTimeInfo(&currentTime)) {
    bool needRefresh = false;

    // 检查时间是否变化
    if (currentTime.tm_hour != lastHour || currentTime.tm_min != lastMinute ||
        forceRefresh) {

      EPD::clearCoreArea(0);

      // 显示大字体时间
      EPD::u8g2.setFont(u8g2_font_helvB24_tr);
      EPD::u8g2.setCursor(COMPONENT_TITLE_X, COMPONENT_TITLE_Y);
      char timeStr[9];
      sprintf(timeStr, "%02d:%02d", currentTime.tm_hour, currentTime.tm_min);
      EPD::u8g2.print(timeStr);

      // 显示日期
      EPD::u8g2.setFont(u8g2_font_helvR14_tr);
      EPD::u8g2.setCursor(COMPONENT_SUBTITLE_X, COMPONENT_SUBTITLE_Y);
      char dateStr[32];
      sprintf(dateStr, "%d-%d-%d", currentTime.tm_year + 1900,
              currentTime.tm_mon + 1, currentTime.tm_mday);
      EPD::u8g2.print(dateStr);

      lastHour = currentTime.tm_hour;
      lastMinute = currentTime.tm_min;
      needRefresh = true;
    }

    if (needRefresh || forceRefresh) {
      EPD::display.display(1);
    }
  }
}

} // namespace Pages