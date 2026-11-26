#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    const int n = sheet.size();
    for (int i = 0; i < n; ++i) {
        const Stroke& s = sheet[i];
        if (s.bell < 0 || s.bell > 8) {
            return Voice{false, Peal()};
        }
        if (s.bell == 0) {
            if (s.beats != 1) {
                return Voice{false, Peal()};
            }
        } else {
            if (s.beats <= 0) {
                return Voice{false, Peal()};
            }
        }
    }

    bool heard[9]{};
    std::vector<Stroke> clean;
    clean.reserve(n);

    for (int i = 0; i < n; ++i) {
        const Stroke& s = sheet[i];
        if (s.bell == 0) {
            for (int b = 1; b <= 8; ++b) {
                heard[b] = false;
            }
            clean.push_back(s);
        } else {
            if (!heard[s.bell]) {
                heard[s.bell] = true;
                clean.push_back(s);
            }
        }
    }

    return Voice{true, Peal(clean.data(), static_cast<int>(clean.size()))};
}

}  // namespace rybinsk