#include "rybinsk/night.hpp"

#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    bool has_last = false;
    size_t last_idx = 0;

    for (size_t i = 0; i < parts.size(); ++i) {
        const Peal& sheet = parts[i];

        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (has_last && sheet == parts[last_idx]) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        has_last = true;
        last_idx = i;

        Voice v = voiceOf(sheet);
        if (!v.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }

        night.played += v.peal;
        night.fates.push_back(SheetFate::Played);
    }

    return night;
}

}  // namespace rybinsk