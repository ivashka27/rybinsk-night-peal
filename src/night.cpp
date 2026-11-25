#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;

    bool has_last = false;
    Peal last_seen;

    for (int i = 0; i < static_cast<int>(parts.size()); ++i) {
        const Peal& sheet = parts[i];

        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (has_last && sheet == last_seen) {
            night.fates.push_back(SheetFate::Repeat);
            last_seen = sheet;
            continue;
        }

        Voice voice = voiceOf(sheet);
        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            last_seen = sheet;
            has_last = true;
            continue;
        }

        night.played += voice.peal;
        night.fates.push_back(SheetFate::Played);
        last_seen = sheet;
        has_last = true;
    }

    return night;
}

}