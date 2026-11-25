#include "rybinsk/voice.hpp"

#include <array>
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    constexpr int last_bell = 8;
    std::array<bool, last_bell + 1> heard{};
    std::vector<Stroke> cleaned;

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell < 0 || stroke.bell > last_bell || stroke.beats < 1 ||
            (stroke.bell == 0 && stroke.beats != 1)) {
            return {false, Peal{}};
        }

        if (stroke.bell == 0) {
            heard.fill(false);
            cleaned.push_back(stroke);
        } else if (!heard[stroke.bell]) {
            heard[stroke.bell] = true;
            cleaned.push_back(stroke);
        }
    }

    return {true, Peal(cleaned.data(), static_cast<int>(cleaned.size()))};
}

}  // namespace rybinsk
