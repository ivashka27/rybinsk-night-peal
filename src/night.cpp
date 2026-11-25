#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    night.fates.reserve(parts.size());
    const Peal* previous = nullptr;
    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (previous != nullptr && sheet == *previous) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }
        previous = &sheet;
        const Voice voice = voiceOf(sheet);
        if (voice.accepted) {
            night.played += voice.peal;
            night.fates.push_back(SheetFate::Played);
        } else {
            night.fates.push_back(SheetFate::Unfit);
        }
    }
    return night;
}

}  // namespace rybinsk
