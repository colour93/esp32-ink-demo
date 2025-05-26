#include "hitokoto_page.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

namespace Pages {

void HitokotoPage::display() {
  Serial.println("显示一言页");

  const int lineHeight = 20; // 行高

  if (WiFi.status() != WL_CONNECTED) {
    EPD::u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    EPD::u8g2.setCursor(HITOKOTO_PAGE_CONTENT_X, HITOKOTO_PAGE_CONTENT_Y);
    EPD::u8g2.print("网络未连接...");
    EPD::display.display(1);
    return;
  }

  // 获取一言内容
  HTTPClient http;
  http.begin("https://v1.hitokoto.cn/");
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    StaticJsonDocument<4096> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      const char *hitokoto = doc["hitokoto"];
      const char *from = doc["from"];

      EPD::u8g2.setFont(u8g2_font_wqy16_t_gb2312);

      // 显示一言内容（自动换行）
      String text = String(hitokoto);
      int currentY = HITOKOTO_PAGE_CONTENT_Y;
      int pos = 0;
      String currentLine;

      while (pos < text.length()) {
        currentLine = "";
        int lineWidth = 0;

        while (pos < text.length()) {
          String nextChar = text.substring(pos, pos + 3);
          int charWidth = EPD::u8g2.getUTF8Width(nextChar.c_str());

          if (lineWidth + charWidth > HITOKOTO_PAGE_CONTENT_WIDTH) {
            break;
          }

          currentLine += nextChar;
          lineWidth += charWidth;
          pos += nextChar.length();
        }

        // 确保不超出内容区域高度
        if (currentY + lineHeight <=
            HITOKOTO_PAGE_CONTENT_Y + HITOKOTO_PAGE_CONTENT_HEIGHT) {
          EPD::u8g2.setCursor(HITOKOTO_PAGE_CONTENT_X, currentY);
          EPD::u8g2.print(currentLine);
          currentY += lineHeight;
        } else {
          break;
        }
      }
    } else {
      EPD::u8g2.setCursor(HITOKOTO_PAGE_CONTENT_X, HITOKOTO_PAGE_CONTENT_Y);
      EPD::u8g2.print("解析数据失败");
    }
  } else {
    EPD::u8g2.setCursor(HITOKOTO_PAGE_CONTENT_X, HITOKOTO_PAGE_CONTENT_Y);
    EPD::u8g2.print("获取一言失败");
  }

  http.end();
  EPD::display.display(1);
}

} // namespace Pages