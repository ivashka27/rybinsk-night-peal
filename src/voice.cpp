#include "rybinsk/voice.hpp"

#include <array>
#include <vector>

namespace rybinsk {

namespace {

bool isValidStroke(const Stroke& s) {
    const bool is_bell = (s.bell >= 1 && s.bell <= 8 && s.beats >= 1);
    const bool is_pause = (s.bell == 0 && s.beats == 1);
    return is_bell || is_pause;
}

}  // namespace

Voice voiceOf(const Peal& sheet) {
    const int n = sheet.size();
    for (int i = 0; i < n; ++i) {
        if (!isValidStroke(sheet[i])) {
            return Voice{false, Peal{}};
        }
    }

    std::vector<Stroke> filtered;
    filtered.reserve(n);

    std::array<bool, 9> heard{};
    for (int i = 0; i < n; ++i) {
        const Stroke s = sheet[i];
        if (s.bell == 0) {
            heard.fill(false);
            filtered.push_back(s);
        } else if (!heard[s.bell]) {
            heard[s.bell] = true;
            filtered.push_back(s);
        }
    }

    return Voice{true, Peal(filtered.data(), static_cast<int>(filtered.size()))};
}

}  // namespace rybinsk
