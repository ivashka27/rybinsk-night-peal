#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {
namespace {

bool isValidStroke(const Stroke& stroke) {
    if (stroke.bell >= 1 && stroke.bell <= 8 && stroke.beats >= 1) {
        return true;
    }
    if (stroke.bell == 0 && stroke.beats == 1) {
        return true;
    }
    return false;
}

}  // namespace

Voice voiceOf(const Peal& sheet) {
    const int n = sheet.size();
    if (n == 0) {
        return Voice{true, Peal()};
    }

    for (int i = 0; i < n; ++i) {
        if (!isValidStroke(sheet[i])) {
            return Voice{false, Peal()};
        }
    }

    std::vector<Stroke> clean;
    bool sounded[9] = {false};

    for (int i = 0; i < n; ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell == 0) {
            clean.push_back(stroke);
            for (int b = 1; b <= 8; ++b) {
                sounded[b] = false;
            }
        } else {
            if (!sounded[stroke.bell]) {
                clean.push_back(stroke);
                sounded[stroke.bell] = true;
            }
        }
    }

    return Voice{true, Peal(clean.data(), static_cast<int>(clean.size()))};
}

}  // namespace rybinsk
