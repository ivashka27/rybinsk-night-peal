#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    Peal previous;

    for (std::size_t i = 0; i < parts.size(); i++) {
        if (parts[i].size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (parts[i] == previous) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        previous = parts[i];

        Voice voice = voiceOf(parts[i]);

        if (voice.accepted == false) {
            result.fates.push_back(SheetFate::Unfit);
            continue;
        }

        result.played += voice.peal;
        result.fates.push_back(SheetFate::Played);
    }

    return result;
}

} //namespace rybinsk