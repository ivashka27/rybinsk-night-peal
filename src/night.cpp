#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

    Night ringNight(const std::vector<Peal>& parts) {
        Night night;
        Peal last_considered;
        bool has_last = false;

        for (const auto& part : parts) {
            if (part.size() == 0) {
                night.fates.push_back(SheetFate::Empty);
                continue;
            }

            if (has_last && part == last_considered) {
                night.fates.push_back(SheetFate::Repeat);
                continue;
            }

            last_considered = part;
            has_last = true;

            Voice v = voiceOf(part);
            if (!v.accepted) {
                night.fates.push_back(SheetFate::Unfit);
                continue;
            }

            night.played += v.peal;
            night.fates.push_back(SheetFate::Played);
        }

        return night;
    }

}
