#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
  std::vector<Stroke> filtered;
  filtered.reserve(sheet.size());

  bool seen[9] = {false};

  for (int i=0; i < sheet.size();++i) {
    const Stroke& s = sheet[i];
    bool is_rest = (s.bell == 0 && s.beats == 1);
    bool is_bell = (s.bell >= 1 && s.bell <= 8 && s.beats >= 1);

    if (!is_rest && !is_bell) {
      return Voice{false, Peal()};
    }

    if (is_rest) {
      for (int b = 1; b <= 8; ++b) {
        seen[b] = false;
      }
      filtered.push_back(s);
    } else {
      if (!seen[s.bell]) {
        seen[s.bell] = true;
        filtered.push_back(s);
      }
    }
  }

  Peal result(filtered.data(), static_cast<int>(filtered.size()));
  
  return Voice{true, std::move(result)};
}

} // namespace rybinsk
