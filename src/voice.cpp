#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    std::vector<Stroke> result;
    bool seen[9] = {};

    for (int i = 0; i < sheet.size(); i++) {
        Stroke s = sheet[i];

        if (s.bell < 0 || s.bell > 8)
            return {false, Peal()};

        if (s.beats < 1)
            return {false, Peal()};

        if (s.bell == 0 && s.beats != 1)
            return {false, Peal()};

        if (s.bell == 0) {
            result.push_back(s);

            for (int j = 0; j < 9; j++)
                seen[j] = false;
        }
        else if (seen[s.bell] == false) {
            result.push_back(s);
            seen[s.bell] = true;
        }
    }

    return {true, Peal(result.data(), result.size())};
}

}
