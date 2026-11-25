#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {
namespace {

constexpr int kBreath = 0;
constexpr int kLastBell = 8;

bool fits(const Stroke& stroke) {
    if (stroke.bell == kBreath) {
        return stroke.beats == 1;
    }
    return stroke.bell >= 1 && stroke.bell <= kLastBell && stroke.beats >= 1;
}

}

Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); ++i) {
        if (!fits(sheet[i])) {
            return Voice{false, Peal()};
        }
    }

    std::vector<Stroke> kept;
    bool heard[kLastBell + 1] = {};
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell == kBreath) {
            for (bool& bell : heard) {
                bell = false;
            }
            kept.push_back(stroke);
        } else if (!heard[stroke.bell]) {
            heard[stroke.bell] = true;
            kept.push_back(stroke);
        }
    }
    return Voice{true, Peal(kept.data(), static_cast<int>(kept.size()))};
}

}  // namespace rybinsk
