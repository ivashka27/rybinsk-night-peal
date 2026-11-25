#include "rybinsk/voice.hpp"

#include <array>
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    std::array<bool, 9> heard{};
    std::vector<Stroke> kept;
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell == 0) {
            if (stroke.beats != 1) {
                return {false, {}};
            }
            heard.fill(false);
        } else {
            if (stroke.bell < 1 || stroke.bell > 8 || stroke.beats < 1) {
                return {false, {}};
            }
            if (heard[stroke.bell]) {
                continue;
            }
            heard[stroke.bell] = true;
        }
        kept.push_back(stroke);
    }
    return {true, Peal(kept.data(), static_cast<int>(kept.size()))};
}

}  // namespace rybinsk
