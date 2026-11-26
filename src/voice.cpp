#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    std::vector<Stroke> result;
    int lastBell = 0;
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell < 0 ||
            stroke.bell > 8 ||
            stroke.beats < 1 ||
            (stroke.bell == 0 && stroke.beats != 1)) {
            return Voice{false, Peal()};
        }
        if (stroke.bell == 0) {
            result.push_back(stroke);
            lastBell = 0;
        } else if (stroke.bell != lastBell) {
            result.push_back(stroke);
            lastBell = stroke.bell;
        }
    }
    return Voice{
        true,
        Peal(result.data(), static_cast<int>(result.size()))
    };
}

}  // namespace rybinsk