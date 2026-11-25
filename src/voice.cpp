#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    if (sheet.size() == 0) {
        return {true, Peal()};
    }

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];
        if (s.bell == 0) {
            if (s.beats != 1) {
                return {false, Peal()};
            }
        } else if (s.bell >= 1 && s.bell <= 8) {
            if (s.beats <= 0) {
                return {false, Peal()};
            }
        } else {
            return {false, Peal()};
        }
    }

    std::vector<Stroke> filtered;
    bool heard[9] = {false};

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];
        if (s.bell == 0) {
            filtered.push_back(s);
            for (int b = 1; b <= 8; ++b) {
                heard[b] = false;
            }
        } else {
            if (!heard[s.bell]) {
                filtered.push_back(s);
                heard[s.bell] = true;
            }
        }
    }

    return {true, Peal(filtered.data(), static_cast<int>(filtered.size()))};
}

}  // namespace rybinsk
