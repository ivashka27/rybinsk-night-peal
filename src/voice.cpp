#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    bool heard[9] = {};
    std::vector<Stroke> result;

    for (int i = 0; i < sheet.size(); ++i) {
        Stroke stroke = sheet[i];

        if (stroke.bell == 0 && stroke.beats == 1) {
            result.push_back(stroke);
            for (int bell = 1; bell <= 8; ++bell) {
                heard[bell] = false;
            }
            continue;
        }

        if (stroke.bell < 1 || stroke.bell > 8 || stroke.beats < 1) {
            return {false, Peal()};
        }

        if (!heard[stroke.bell]) {
            result.push_back(stroke);
            heard[stroke.bell] = true;
        }
    }

    return {true, Peal(result.data(), static_cast<int>(result.size()))};
}

}  // namespace rybinsk