#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    bool seen[9] = {};
    std::vector<Stroke> result;

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];

        if (s.bell < 0 || s.bell > 8 || s.beats < 1 ||
            (s.bell == 0 && s.beats != 1)) {
            return {false, Peal()};
        }

        if (s.bell == 0) {
            result.push_back(s);

            for (bool& value : seen)
                value = false;

        } else if (!seen[s.bell]) {
            seen[s.bell] = true;
            result.push_back(s);
        }
    }

    return {true, Peal(result.data(),
                      static_cast<int>(result.size()))};
}

}