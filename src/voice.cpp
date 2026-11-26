#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    std::vector<Stroke> strokes;
    bool seen[9] = {};
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];
        if (s.bell < 0 || s.bell > 8 || s.beats < 1 ||
            (s.bell == 0 && s.beats != 1)) {
            return {false, Peal()};
        }

        if (s.bell == 0) {
            strokes.push_back(s);
            for (bool& bell : seen) {
                bell = false;
            }
        } else if (!seen[s.bell]) {
            strokes.push_back(s);
            seen[s.bell] = true;
        }
    }
    return {true, Peal(strokes.data(), static_cast<int>(strokes.size()))};
}

}  // namespace rybinsk
