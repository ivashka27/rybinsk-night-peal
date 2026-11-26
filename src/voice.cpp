#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    std::vector<Stroke> kept;
    bool heard[9] = {};
    for (int i = 0; i < sheet.size(); i++) {
        Stroke s = sheet[i];
        if (s.bell == 0 && s.beats == 1) {
            kept.push_back(s);
            for (int b = 1; b <= 8; b++) {
                heard[b] = false;
            }
        } else if (s.bell >= 1 && s.bell <= 8 && s.beats >= 1) {
            if (!heard[s.bell]) {
                kept.push_back(s);
                heard[s.bell] = true;
            }
        } else {
            return {false, Peal()};
        }
    }
    return {true, Peal(kept.data(), static_cast<int>(kept.size()))};
}

}  // namespace rybinsk