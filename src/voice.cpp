#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    std::vector<Stroke> result;
    bool heard[9]{};

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        if (stroke.bell < 0 || stroke.bell > 8 || stroke.beats < 1 ||
            (stroke.bell == 0 && stroke.beats != 1)) {
            return {false, Peal()};
        }

        if (stroke.bell == 0) {
            for (int bell = 1; bell <= 8; ++bell) {
                heard[bell] = false;
            }
            result.push_back(stroke);
        } else if (!heard[stroke.bell]) {
            heard[stroke.bell] = true;
            result.push_back(stroke);
        }
    }
    return {true, Peal(result.data(), static_cast<int>(result.size()))};
}

}  // namespace rybinsk