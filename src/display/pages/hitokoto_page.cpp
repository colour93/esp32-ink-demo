#include "hitokoto_page.h"

namespace Pages {

void HitokotoPage::display() {
  Serial.println("显示一言页");

  EPD::u8g2.setFont(u8g2_font_helvB24_tr);
  EPD::u8g2.setCursor(COMPONENT_TITLE_X, COMPONENT_TITLE_Y);
  EPD::u8g2.print("Hitokoto");

  EPD::u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  EPD::u8g2.setCursor(COMPONENT_SUBTITLE_X, COMPONENT_SUBTITLE_Y);
  EPD::u8g2.print("一言");

  EPD::display.display(1);
}

} // namespace Pages