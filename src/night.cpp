#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    Peal previous;

    for (const Peal& part : parts) {
        if (part.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (part == previous) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        previous = part;

        Voice voice = voiceOf(part);

        if (voice.accepted == false) {
            result.fates.push_back(SheetFate::Unfit);
        } else {
            result.fates.push_back(SheetFate::Played);
            result.played += voice.peal;
        }
    }

    return result;
}

}  // namespace rybinsk
