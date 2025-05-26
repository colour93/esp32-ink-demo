#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "../epd.h"

#include "data_page.h"
#include "welcome_page.h"
// #include "animation_page.h"
#include "hitokoto_page.h"

namespace Pages {

enum PageType {
  WELCOME = 0,
  DATA,
  // ANIMATION,
  HITOKOTO,
  PAGE_COUNT // 用于计算总页数
};

extern PageType currentPage;

void setup();
void switchToNextPage();
void updateCurrentPage();

} // namespace Pages

#endif
