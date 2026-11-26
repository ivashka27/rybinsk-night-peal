#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& peal) {
    Voice result;
    result.accepted = true;
    result.peal = Peal();

    std::vector<Stroke> valid_strokes;
    std::vector<bool> bell_played(9, false);

    for (int i = 0; i < peal.size(); ++i) {
        const Stroke& s = peal[i];
        bool is_valid = false;
        if (s.bell == 0 && s.beats == 1) {
            is_valid = true;
        } else if (s.bell >= 1 && s.bell <= 8 && s.beats >= 1) {
            is_valid = true;
        }

        if (!is_valid) {
            result.accepted = false;
            return result;
        }
    }

    for (int i = 0; i < peal.size(); ++i) {
        const Stroke& s = peal[i];
        if (s.bell == 0) {
            valid_strokes.push_back(s);
            for (int b = 0; b <= 8; ++b) bell_played[b] = false;
        } else {
            if (!bell_played[s.bell]) {
                valid_strokes.push_back(s);
                bell_played[s.bell] = true;
            }
        }
    }

    if (valid_strokes.empty()) {
        result.peal = Peal();
    } else {
        result.peal = Peal(valid_strokes.data(), static_cast<int>(valid_strokes.size()));
    }

    return result;
}

}  // namespace rybinsk