#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

    Night ringNight(const std::vector<Peal>& parts) {
        Night result;
        result.fates.reserve(parts.size());

        bool has_last = false;
        Peal last_non_empty;

        for (const Peal& part : parts) {
            if (part.size() == 0) {
                result.fates.push_back(SheetFate::Empty);
                continue;
            }

            if (has_last && part == last_non_empty) {
                result.fates.push_back(SheetFate::Repeat);
                continue;
            }

            has_last = true;
            last_non_empty = part;

            Voice v = voiceOf(part);
            if (!v.accepted) {
                result.fates.push_back(SheetFate::Unfit);
            }
            else {
                result.fates.push_back(SheetFate::Played);
                result.played += v.peal;
            }
        }

        return result;
    }

} // namespace rybinsk
