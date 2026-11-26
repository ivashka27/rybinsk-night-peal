#include "rybinsk/voice.hpp"

#include <algorithm>
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell < 0 || stroke.bell > 8 ||
            (stroke.bell == 0 ? stroke.beats != 1 : stroke.beats < 1)) {
            return {false, Peal()};
        }
    }

    bool heard[9]{};
    std::vector<Stroke> strokes;
    strokes.reserve(sheet.size());
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell == 0) {
            strokes.push_back(stroke);
            std::fill(std::begin(heard), std::end(heard), false);
        } else if (!heard[stroke.bell]) {
            strokes.push_back(stroke);
            heard[stroke.bell] = true;
        }
    }
    return {true, Peal(strokes.data(), static_cast<int>(strokes.size()))};
}

}  // namespace rybinsk
