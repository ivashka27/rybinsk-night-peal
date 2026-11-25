#include "rybinsk/voice.hpp"
#include <vector>
namespace rybinsk {
namespace {
bool isValidStroke(const Stroke& s) {
    if (s.bell == 0) {
        return s.beats == 1;
    }
    return (s.bell >= 1 && s.bell <= 8) && (s.beats >= 1);
}
}  // namespace
Voice voiceOf(const Peal& sheet) {
    const int n = sheet.size();
    for (int i = 0; i < n; ++i) {
        if (!isValidStroke(sheet[i])) {
            return Voice{false, Peal()};
        }
    }
    bool heard[9] = {false};
    std::vector<Stroke> filtered;
    filtered.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        const Stroke s = sheet[i];
        if (s.bell == 0) {
            for (int b = 1; b <= 8; ++b) {
                heard[b] = false;
            }
            filtered.push_back(s);
        } else if (!heard[s.bell]) {
            heard[s.bell] = true;
            filtered.push_back(s);
        }
    }
    return Voice{true, Peal(filtered.data(), static_cast<int>(filtered.size()))};
}
}  // namespace rybinsk
