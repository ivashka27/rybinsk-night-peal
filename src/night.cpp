#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& ps) {
    Night night;
    night.fates.reserve(ps.size());

    Peal last;
    bool have_last = false;

    for (const Peal& p : ps) {
        if (p.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (have_last && p == last) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        last = p;
        have_last = true;
        Voice voice = voiceOf(p);
        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }
        night.played += voice.peal;
        night.fates.push_back(SheetFate::Played);
    }
    return night;
}

}  // namespace rybinsk
