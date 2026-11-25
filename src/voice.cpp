#include "rybinsk/voice.hpp"
#include <vector>
namespace rybinsk {



Voice voiceOf(const Peal& sheet) {
    std::vector<Stroke> result;
    int used[9] = {};

    for (int i = 0; i < sheet.size(); i++) {
        Stroke stroke = sheet[i];

        int bell = stroke.bell;
        int beats = stroke.beats;

        if (bell < 0 || bell > 8 || beats < 1) {
            return {false, Peal()};
        }

        if (bell == 0 && beats != 1) {
            return {false, Peal()};
        }

        if (bell == 0) {
            result.push_back(stroke);

            for (int j = 0; j < 9; j++) {
                used[j] = 0;
            }
        }
        else if (used[bell] == 0) {
            result.push_back(stroke);
            used[bell] = 1;
        }
    }

    return {true, Peal(result.data(), result.size())};
}
}  // namespace rybinsk
