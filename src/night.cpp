#include "rybinsk/night.hpp"

#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    night.fates.reserve(parts.size());

    const Peal* last_seen = nullptr;
    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (last_seen != nullptr && *last_seen == sheet) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }
        last_seen = &sheet;

        Voice voice = voiceOf(sheet);
        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }
        night.played += voice.peal;
        night.fates.push_back(SheetFate::Played);
    }
    return night;
}

}  // namespace rybinsk
