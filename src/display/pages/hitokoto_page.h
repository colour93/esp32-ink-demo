#ifndef HITOKOTO_PAGE_H
#define HITOKOTO_PAGE_H

#include "../epd.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>

namespace Pages {

class HitokotoPage {
public:
  static void display();
};

} // namespace Pages

#endif
