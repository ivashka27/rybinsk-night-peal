#include "rybinsk/night.hpp"

#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& v) {
    Night r;
    const Peal* l = nullptr;
    for (const Peal& p : v) {
        if (p.size() == 0) {
            r.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (l != nullptr && *l == p) {
            r.fates.push_back(SheetFate::Repeat);
            continue;
        }
        l = &p;
        Voice g = voiceOf(p);
        if (!g.accepted) {
            r.fates.push_back(SheetFate::Unfit);
            continue;
        }
        r.played += g.peal;
        r.fates.push_back(SheetFate::Played);
    }
    return r;
}

}  // namespace rybinsk