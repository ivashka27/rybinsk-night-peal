#include "rybinsk/voice.hpp"

#include <array>
#include <vector>

namespace rybinsk {

    Voice voiceOf(const Peal& sheet) {
        std::array<bool, 9> heard{};
        std::vector<Stroke> strokes;

        for (int i = 0; i < sheet.size(); ++i) {
            const Stroke& stroke = sheet[i];
            if (stroke.bell < 0 || stroke.bell > 8 || stroke.beats < 1 ||
                (stroke.bell == 0 && stroke.beats != 1)) {
                return {false, Peal()};
                }

            if (stroke.bell == 0) {
                heard.fill(false);
            } else {
                if (heard[stroke.bell]) {
                    continue;
                }
                heard[stroke.bell] = true;
            }

            strokes.push_back(stroke);
        }

        return {true, Peal(strokes.data(), static_cast<int>(strokes.size()))};
    }

}  