#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

    Voice voiceOf(const Peal& sheet) {
        const int MIN_BELL = 1;
        const int MAX_BELL = 8;
        const int MIN_BEATS = 1;
        const int PAUSE_BELL = 0;
        const int PAUSE_BEATS = 1;

        for (int i = 0; i < sheet.size(); ++i) {
            Stroke s = sheet[i];
            bool is_bell = (s.bell >= MIN_BELL && s.bell <= MAX_BELL && s.beats >= MIN_BEATS);
            bool is_pause = (s.bell == PAUSE_BELL && s.beats == PAUSE_BEATS);

            if (!is_bell && !is_pause) {
                return {false, Peal()};
            }
        }

        std::vector<Stroke> cleaned;
        int last_bell = -1;

        for (int i = 0; i < sheet.size(); ++i) {
            Stroke s = sheet[i];
            if (s.bell == PAUSE_BELL) {
                cleaned.push_back(s);
                last_bell = -1;
            } else {
                if (s.bell != last_bell) {
                    cleaned.push_back(s);
                    last_bell = s.bell;
                }
            }
        }

        return {true, Peal(cleaned.data(), static_cast<int>(cleaned.size()))};
    }

}
