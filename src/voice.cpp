#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];

        bool normal =
            stroke.bell >= 1 &&
            stroke.bell <= 8 &&
            stroke.beats >= 1;

        bool rest =
            stroke.bell == 0 &&
            stroke.beats == 1;

        if (!normal && !rest) {
            return {false, Peal()};
        }
    }

    bool heard[9] = {};
    std::vector<Stroke> filtered;

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];

        if (stroke.bell == 0) {
            filtered.push_back(stroke);

            for (int j = 1; j <= 8; ++j) {
                heard[j] = false;
            }
        } else if (!heard[stroke.bell]) {
            filtered.push_back(stroke);
            heard[stroke.bell] = true;
        }
    }

    return {
        true,
        Peal(filtered.data(),
             static_cast<int>(filtered.size()))
    };
}

}  // namespace rybinsk
