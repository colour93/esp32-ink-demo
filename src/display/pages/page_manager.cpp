#include "page_manager.h"

namespace Pages {

PageType currentPage = WELCOME;

void setup() {
  Serial.println("显示欢迎页");

  // 清除核心显示区域
  EPD::clearCoreArea(1);
  // 初始化显示欢迎页
  updateCurrentPage();
}

void switchToNextPage() {
  Serial.println("切换到下一页");

  // 清除核心显示区域
  EPD::clearCoreArea(1);

  // 切换到下一页
  currentPage = static_cast<PageType>((currentPage + 1) % PAGE_COUNT);

  // 更新显示
  updateCurrentPage();
}

void updateCurrentPage() {
  switch (currentPage) {
  case WELCOME:
    WelcomePage::display(1);
    break;
  case DATA:
    DataPage::display(1);
    break;
  // case ANIMATION:
  //     AnimationPage::display();
  //     break;
  case HITOKOTO:
    HitokotoPage::display();
    break;
  default:
    break;
  }
}

} // namespace Pages