#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    night.fates.reserve(parts.size());

    const Peal* last_non_empty = nullptr;

    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (last_non_empty != nullptr && sheet == *last_non_empty) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        last_non_empty = &sheet;

        Voice voice = voiceOf(sheet);
        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }

        night.fates.push_back(SheetFate::Played);
        night.played += voice.peal;
    }

    return night;
}

}  // namespace rybinsk
