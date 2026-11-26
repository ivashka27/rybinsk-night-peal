#include "rybinsk/voice.hpp"

#include <array>
#include <vector>

namespace rybinsk {

    Voice voiceOf(const Peal& sheet) {
        std::array<bool, 9> heard{};
        std::vector<Stroke> strokes;

        for (int i = 0; i < sheet.size(); ++i) {
            const Stroke& stroke = sheet[i];

            if (stroke.bell == 0 && stroke.beats == 1) {
                strokes.push_back(stroke);
                heard.fill(false);
                continue;
            }

            if (stroke.bell < 1 || stroke.bell > 8 || stroke.beats < 1) {
                return Voice{false, Peal()};
            }

            if (!heard[stroke.bell]) {
                strokes.push_back(stroke);
                heard[stroke.bell] = true;
            }
        }

        Peal result(strokes.data(), static_cast<int>(strokes.size()));
        return Voice{true, static_cast<Peal&&>(result)};
    }

}  // namespace rybinsk