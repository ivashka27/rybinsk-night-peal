#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& s) {
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        const Stroke& k = s[i];
        bool o = (k.bell >= 1 && k.bell <= 8 && k.beats >= 1) ||
                 (k.bell == 0 && k.beats == 1);
        if (!o) return Voice{false, Peal()};
    }
    std::vector<Stroke> v;
    bool e[9] = {};
    for (int i = 0; i < n; ++i) {
        const Stroke& k = s[i];
        if (k.bell == 0) {
            for (bool& b : e) b = false;
            v.push_back(k);
        } else if (!e[k.bell]) {
            e[k.bell] = true;
            v.push_back(k);
        }
    }
    return Voice{true, Peal(v.data(), static_cast<int>(v.size()))};
}

}  // namespace rybinsk