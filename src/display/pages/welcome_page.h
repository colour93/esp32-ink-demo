#ifndef WELCOME_PAGE_H
#define WELCOME_PAGE_H

#include "../epd.h"
#include "../time_manager.h"

namespace Pages {

class WelcomePage {
public:
  static void display(bool forceRefresh = false);
};

} // namespace Pages

#endif
