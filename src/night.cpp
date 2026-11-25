#include "rybinsk/night.hpp"

#include "rybinsk/voice.hpp"

#include <utility>
#include <vector>

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Peal played;
    std::vector<SheetFate> fates;
    fates.reserve(parts.size());

    const Peal* lastNonEmpty = nullptr;

    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            fates.push_back(SheetFate::Empty);
            continue;
        }

        if (lastNonEmpty != nullptr && sheet == *lastNonEmpty) {
            fates.push_back(SheetFate::Repeat);
            continue;
        }

        lastNonEmpty = &sheet;

        Voice voice = voiceOf(sheet);
        if (!voice.accepted) {
            fates.push_back(SheetFate::Unfit);
            continue;
        }

        played += voice.peal;
        fates.push_back(SheetFate::Played);
    }

    return {std::move(played), std::move(fates)};
}

}  // namespace rybinsk
