#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    int n = sheet.size();

    for (int i = 0; i < n; ++i) {
        Stroke s = sheet[i];
        if (s.bell == 0) {
            if (s.beats != 1) {
                return Voice{false, Peal()};
            }
        } else if (s.bell >= 1 && s.bell <= 8) {
            if (s.beats <= 0) {
                return Voice{false, Peal()};
            }
        } else {
            return Voice{false, Peal()};
        }
    }

    bool heard[9];
    for (int b = 0; b < 9; ++b) {
        heard[b] = false;
    }

    std::vector<Stroke> clean;
    for (int i = 0; i < n; ++i) {
        Stroke s = sheet[i];
        if (s.bell == 0) {
            clean.push_back(s);
            for (int b = 1; b <= 8; ++b) {
                heard[b] = false;
            }
        } else {
            if (!heard[s.bell]) {
                clean.push_back(s);
                heard[s.bell] = true;
            }
        }
    }

    return Voice{true, Peal(clean.data(), static_cast<int>(clean.size()))};
}

}  // namespace rybinsk