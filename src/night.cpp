#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    result.fates.reserve(parts.size());

    bool has_last = false;
    Peal last_non_empty;

    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (has_last && sheet == last_non_empty) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        has_last = true;
        last_non_empty = sheet;

        Voice v = voiceOf(sheet);
        if (!v.accepted) {
            result.fates.push_back(SheetFate::Unfit);
            continue;
        }

        result.fates.push_back(SheetFate::Played);
        result.played += v.peal;
    }

    return result;
}

}  // namespace rybinsk