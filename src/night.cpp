#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    int lastIndex = -1;

    for (int i = 0; i < (int)parts.size(); i++) {
        const Peal& part = parts[i];

        if (part.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (lastIndex >= 0 && part == parts[lastIndex]) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        Voice v = voiceOf(part);

        if (v.accepted == false) {
            result.fates.push_back(SheetFate::Unfit);
            lastIndex = i;
            continue;
        }

        result.fates.push_back(SheetFate::Played);
        result.played += v.peal;
        lastIndex = i;
    }

    return result;
}

} // namespace rybinsk