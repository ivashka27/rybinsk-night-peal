#include "rybinsk/voice.hpp"
#include <vector>
namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    int n = sheet.size();
    for (int i = 0; i < n; ++i) {
        Stroke s = sheet[i];
        bool okBell = (s.bell >= 1 && s.bell <= 8 && s.beats >= 1);
        bool okRest = (s.bell == 0 && s.beats == 1);
        if (!okBell && !okRest) {
            Voice bad;
            bad.accepted = false;
            return bad;
        }
}

std::vector<Stroke> res;
    bool seen[9] = {false};

    for (int i = 0; i < n; ++i) {
        Stroke s = sheet[i];
        if (s.bell == 0) {
            for (int j = 0; j < 9; ++j) {
                seen[j] = false;
            }
            res.push_back(s);
        } else {
            if (!seen[s.bell]) {
                seen[s.bell] = true;
                res.push_back(s);
            }
        }
    }

    Voice result;
    result.accepted = true;
    if (!res.empty()) {
        result.peal = Peal(res.data(), (int)res.size());
    }
    return result;
}
}  // namespace rybinsk
