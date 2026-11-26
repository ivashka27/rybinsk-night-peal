#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {
namespace {

constexpr int kMinBell = 1;
constexpr int kMaxBell = 8;

bool isRest(const Stroke& s) {
    return s.bell == 0 && s.beats == 1;
}

bool isBellStroke(const Stroke& s) {
    return s.bell >= kMinBell && s.bell <= kMaxBell && s.beats >= 1;
}

}  // namespace

Voice voiceOf(const Peal& sheet) {
    const int n = sheet.size();

    // Один плохой удар портит листок целиком, где бы он ни стоял.
    for (int i = 0; i < n; ++i) {
        const Stroke& s = sheet[i];
        if (!isRest(s) && !isBellStroke(s)) {
            return Voice{false, Peal()};
        }
    }

    // Снимаем эхо: пока не было передышки, второй удар в тот же колокол
    // выбрасывается. Передышка стирает память о колоколах и остаётся сама.
    std::vector<Stroke> kept;
    kept.reserve(static_cast<std::size_t>(n));
    bool heard[kMaxBell + 1] = {};

    for (int i = 0; i < n; ++i) {
        const Stroke& s = sheet[i];
        if (isRest(s)) {
            kept.push_back(s);
            for (bool& h : heard) {
                h = false;
            }
        } else if (!heard[s.bell]) {
            heard[s.bell] = true;
            kept.push_back(s);
        }
    }

    return Voice{true, Peal(kept.data(), static_cast<int>(kept.size()))};
}

}  // namespace rybinsk
