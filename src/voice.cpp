#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

    Voice voiceOf(const Peal& sheet) {
        int n = sheet.size();
        for (int i = 0; i < n; ++i) {
            const Stroke& s = sheet[i];
            bool is_bell = (s.bell >= 1 && s.bell <= 8 && s.beats >= 1);
            bool is_rest = (s.bell == 0 && s.beats == 1);
            if (!is_bell && !is_rest) {
                return Voice{ false, Peal() };
            }
        }

        bool sounded[9] = { false };
        std::vector<Stroke> filtered;
        filtered.reserve(n);

        for (int i = 0; i < n; ++i) {
            const Stroke& s = sheet[i];
            if (s.bell == 0) {
                for (int b = 1; b <= 8; ++b) {
                    sounded[b] = false;
                }
                filtered.push_back(s);
            }
            else {
                if (!sounded[s.bell]) {
                    sounded[s.bell] = true;
                    filtered.push_back(s);
                }
            }
        }

        if (filtered.empty()) {
            return Voice{ true, Peal() };
        }
        return Voice{ true, Peal(filtered.data(), static_cast<int>(filtered.size())) };
    }

} // namespace rybinsk
