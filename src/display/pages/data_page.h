#ifndef DATA_PAGE_H
#define DATA_PAGE_H

#include "../epd.h"
#include "../store.h"
namespace Pages {

class DataPage {
public:
  static void display(bool forceRefresh = false);
};

} // namespace Pages

#endif
