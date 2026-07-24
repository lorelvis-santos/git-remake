#include "core/StringUtils.h"

namespace StringUtils {
  bool starts_with(const std::string_view text, const std::string_view to_find) {
    return text.compare(0, to_find.size(), to_find) == 0;
  }
}