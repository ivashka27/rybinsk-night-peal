#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    night.fates.reserve(parts.size());

    bool has_last = false;
    Peal last_considered;

    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (has_last && sheet == last_considered) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        last_considered = sheet;
        has_last = true;

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
