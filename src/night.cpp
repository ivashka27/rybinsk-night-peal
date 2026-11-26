#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    const Peal* previous = nullptr;

    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (previous && sheet == *previous) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        previous = &sheet;

        Voice voice = voiceOf(sheet);

        if (!voice.accepted) {
            result.fates.push_back(SheetFate::Unfit);
        } else {
            result.played += voice.peal;
            result.fates.push_back(SheetFate::Played);
        }
    }

    return result;
}

}