#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

    Night ringNight(const std::vector<Peal>& peals) {
        Night res{Peal(), {}};

        Peal prev;
        bool hasPrev = false;

        for (const Peal& p : peals) {
            if (p.size() == 0) {
                res.fates.push_back(SheetFate::Empty);
                continue;
            }

            if (hasPrev && p == prev) {
                res.fates.push_back(SheetFate::Repeat);
                continue;
            }

            prev = p;
            hasPrev = true;

            Voice v = voiceOf(p);

            if (!v.accepted) {
                res.fates.push_back(SheetFate::Unfit);
                continue;
            }

            res.fates.push_back(SheetFate::Played);
            res.played += v.peal;
        }

        return res;
    }

}