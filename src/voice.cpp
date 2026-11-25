#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); i++) {
        const int bell = sheet[i].bell;
        const int beats = sheet[i].beats;
        const bool good = (bell >= 1 && bell <= 8 && beats >= 1) || (bell == 0 && beats == 1);
        if (!good) {
            return Voice{false, Peal()};
        }
    }

    bool heard[9] = {};
    std::vector<Stroke> result;

    for (int i = 0; i < sheet.size(); i++) {
        const Stroke stroke = sheet[i];
        if (stroke.bell == 0) {
            result.push_back(stroke);
            for (int j = 0; j < 9; j++) {
                heard[j] = false;
            }
        } else if (!heard[stroke.bell]) {
            heard[stroke.bell] = true;
            result.push_back(stroke);
        }
    }
    return Voice{true, Peal(result.data(), static_cast<int>(result.size()))};
}
}  // namespace rybinsk
