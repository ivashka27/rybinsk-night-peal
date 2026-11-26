#include "rybinsk/voice.hpp"
#include <vector>
namespace rybinsk {
Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); ++i) {
        Stroke str = sheet[i];
        if (str.bell == 0 && str.beats == 1) {
            continue;
        }
        if (str.bell >= 1 && str.bell <= 8 && str.beats >= 1) {
            continue;
        }
        return Voice{false, Peal()};
    }
    std::vector<Stroke> result;
    std::vector<bool> been(9, false);
    for (int i = 0; i < sheet.size(); ++i) {
        Stroke str = sheet[i];
        if (str.bell == 0) {
            result.push_back(str);
            for (int j = 1; j <= 8; ++j) {
                been[j] = false;
            }
        } else if (!been[str.bell]) {
            been[str.bell] = true;
            result.push_back(str);
        }
    }
    return Voice{true, Peal(result.data(), result.size())};
}
}  // namespace rybinsk
