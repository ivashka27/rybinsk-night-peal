#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
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