#include "theme.h"

namespace tabos {
namespace theme {

const char* pageTitle(uint8_t page) {
  switch (page) {
    case 0:
      return "Core";
    case 1:
      return "IT Tools";
    case 2:
      return "Hardware";
    case 3:
      return "System";
    case 4:
      return "Utilities";
    default:
      return "Apps";
  }
}

}  // namespace theme
}  // namespace tabos
