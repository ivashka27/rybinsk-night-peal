#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal&sheet) {
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];

        bool good;
        if (s.bell == 0) {
            good = (s.beats == 1);
        } else {
            good = (s.bell >= 1 && s.bell <= 8 && s.beats >= 1);
        }

        if (!good) {
            return Voice{false, Peal()};
        }
    }

    bool seen[9] = {false};
    int keep = 0;
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];

        if (s.bell == 0) {
            ++keep;
            for (int b = 0; b < 9; ++b) {
                seen[b] = false;
            }
        } else if (!seen[s.bell]) {
            ++keep;
            seen[s.bell] = true;
        }
    }

    Peal result(keep);
    for (int b = 0; b < 9; ++b) {
        seen[b] = false;
    }
    int k = 0;
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];

        if (s.bell == 0) {
            result[k++] = s;
            for (int b = 0; b < 9; ++b) {
                seen[b] = false;
            }
        } else if (!seen[s.bell]) {
            result[k++] = s;
            seen[s.bell] = true;
        }
    }

    return Voice{true, std::move(result)};
}

}  // namespace rybinsk
