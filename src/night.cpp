#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& stack) {
    Night night;
    const Peal* last_considered = nullptr;

    for (const Peal& p : stack) {
        if (p.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (last_considered && p == *last_considered) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        last_considered = &p;

        Voice v = voiceOf(p);
        if (!v.accepted) {
            night.fates.push_back(SheetFate::Unfit);
        } else {
            night.fates.push_back(SheetFate::Played);
            night.played += v.peal;
        }
    }

    return night;
}

}  // namespace rybinsk