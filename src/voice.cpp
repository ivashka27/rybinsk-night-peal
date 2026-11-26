#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

    Voice voiceOf(const Peal& sheet) {
        Voice result{ false, Peal() };
        const int n = sheet.size();

        for (int i = 0; i < n; ++i) {
            const Stroke& s = sheet[i];
            const bool isBell = s.bell >= 1 && s.bell <= 8 && s.beats >= 1;
            const bool isBreath = s.bell == 0 && s.beats == 1;
            if (!isBell && !isBreath) {
                return result;
            }
        }

        std::vector<Stroke> buf;
        buf.reserve(static_cast<std::size_t>(n));

        bool seen[9] = { false };
        for (int i = 0; i < n; ++i) {
            const Stroke& s = sheet[i];
            if (s.bell == 0) {
                buf.push_back(s);
                for (bool& b : seen) {
                    b = false;
                }
            }
            else {
                if (seen[s.bell]) {
                    continue;
                }
                seen[s.bell] = true;
                buf.push_back(s);
            }
        }

        result.accepted = true;
        result.peal = Peal(buf.data(), static_cast<int>(buf.size()));
        return result;
    }

}  // namespace rybinsk
