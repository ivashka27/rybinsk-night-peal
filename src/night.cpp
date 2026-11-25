
#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night{Peal(), {}};

    night.fates.reserve(parts.size());

    Peal previous;
    bool hasPrevious = false;

    for (const Peal& part : parts) {
        if (part.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (hasPrevious && part == previous) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        previous = part;
        hasPrevious = true;

        Voice voice = voiceOf(part);

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
