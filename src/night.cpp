#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    night.fates.reserve(parts.size());

    const Peal* last_considered = nullptr;

    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (last_considered != nullptr && sheet == *last_considered) {
            night.fates.push_back(SheetFate::Repeat);
            last_considered = &sheet;
            continue;
        }

        last_considered = &sheet;
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
