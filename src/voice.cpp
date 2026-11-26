#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];

        if (s.bell < 0 || s.bell > 8 || s.beats < 1) {
            return {false, Peal()};
        }

        if (s.bell == 0 && s.beats != 1) {
            return {false, Peal()};
        }
    }

    Peal result(sheet.size());
    int k = 0;
    int lastBell = -1;

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& s = sheet[i];

        if (s.bell == 0) {
            result[k++] = s;
            lastBell = -1;
        } else {
            if (s.bell != lastBell) {
                result[k++] = s;
            }

            lastBell = s.bell;
        }
    }

    Peal voice(k);

    for (int i = 0; i < k; ++i) {
        voice[i] = result[i];
    }

    return {true, std::move(voice)};
}

}  // namespace rybinsk