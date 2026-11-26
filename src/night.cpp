#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;

    Peal previous;
    bool hasPrevious = false;

    for (int i = 0; i < static_cast<int>(parts.size()); i++) {
        const Peal& part = parts[i];

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
        } else {
            night.fates.push_back(SheetFate::Played);
            night.played += voice.peal;
        }
    }

    return night;
}

}  // namespace rybinsk
