#include "rybinsk/voice.hpp"

#include <algorithm>
#include <vector>

namespace rybinsk {
namespace {

bool isValidStroke(const Stroke& stroke) {
    if (stroke.bell == 0) {
        return stroke.beats == 1;
    }
    return stroke.bell >= 1 && stroke.bell <= 8 && stroke.beats >= 1;
}

}  // namespace

Voice voiceOf(const Peal& sheet) {
    const int n = sheet.size();
    std::vector<Stroke> filtered;
    filtered.reserve(n);

    bool seen[9]{};
    for (int i = 0; i < n; ++i) {
        const Stroke& s = sheet[i];
        if (!isValidStroke(s)) {
            return Voice{false, Peal()};
        }
        if (s.bell == 0) {
            std::fill(std::begin(seen), std::end(seen), false);
            filtered.push_back(s);
        } else if (!seen[s.bell]) {
            seen[s.bell] = true;
            filtered.push_back(s);
        }
    }

    return Voice{true, Peal(filtered.data(), static_cast<int>(filtered.size()))};
}

}  // namespace rybinsk
