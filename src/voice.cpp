#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {
namespace {
    bool is_rught_stroke(const Stroke& s) {
        if (s.bell == 0) {
            return s.beats == 1;
        }
        return s.bell >= 1 && s.bell <= 8 && s.beats >= 1;
    }
}

Voice voiceOf(const Peal& p) {
    for (int i = 0; i < p.size(); i++) {
        if (!is_rught_stroke(p[i])) {
            return Voice{false, Peal()};
        }
    }

    std::vector<Stroke> out;
    bool already_rang[9] = {};

    for (int i = 0; i < p.size(); i++) {
        const Stroke s = p[i];
        if (s.bell == 0) {
            for (int b = 1; b <= 8; b++) {
                already_rang[b] = false;
            }
            out.push_back(s);
        } else if (!already_rang[s.bell]) {
            already_rang[s.bell] = true;
            out.push_back(s);
        }
    }

    return Voice{true, Peal(out.data(), static_cast<int>(out.size()))};
}

}  // namespace rybinsk
