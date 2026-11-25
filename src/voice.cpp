#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

namespace {

bool isGoodStroke(const Stroke& stroke) {
    if (stroke.bell == 0) {
        return stroke.beats == 1;
    }
    if (stroke.bell >= 1 && stroke.bell <= 8) {
        return stroke.beats >= 1;
    }
    return false;
}

}

Voice voiceOf(const Peal& sheet) {
    Voice result;
    result.accepted = false;

    const int n = sheet.size();
    for (int i = 0; i < n; ++i) {
        if (!isGoodStroke(sheet[i])) {
            return result;
        }
    }

    result.accepted = true;

    std::vector<Stroke> cleaned;
    bool seen[9] = {false};
    for (int i = 0; i < n; ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell == 0) {
            cleaned.push_back(stroke);
            for (int b = 0; b < 9; ++b) {
                seen[b] = false;
            }
        } else {
            if (!seen[stroke.bell]) {
                cleaned.push_back(stroke);
                seen[stroke.bell] = true;
            }
        }
    }

    result.peal = Peal(cleaned.data(), static_cast<int>(cleaned.size()));
    return result;
}

}