#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    Peal previous;
    bool has_previous = false;
    for (int i = 0; i < static_cast<int>(parts.size()); i++) {
        const Peal& current = parts[i];
        if (current.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (has_previous && current == previous) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }
        previous = current;
        has_previous = true;
        Voice voice = voiceOf(current);
        if (!voice.accepted) {
            result.fates.push_back(SheetFate::Unfit);
        } else {
            result.fates.push_back(SheetFate::Played);
            result.played += voice.peal;
        }
    }
    return result;
}
}