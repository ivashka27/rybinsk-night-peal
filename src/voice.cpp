#include "rybinsk/voice.hpp"
#include <array>
#include <utility>
#include <vector>

namespace rybinsk {

    Voice voiceOf(const Peal& p) {
        std::array<bool, 9> heard{};
        std::vector<Stroke> result;

        for (int i = 0; i < p.size(); ++i) {
            const Stroke& s = p[i];

            if (s.bell == 0 && s.beats == 1) {
                result.push_back(s);
                heard.fill(false);
                continue;
            }

            if (s.bell < 1 || s.bell > 8 || s.beats < 1) {
                return Voice{false, Peal()};
            }

            if (!heard[s.bell]) {
                result.push_back(s);
                heard[s.bell] = true;
            }
        }

        Peal peal(result.data(), static_cast<int>(result.size()));
        return Voice{true, std::move(peal)};
    }
}