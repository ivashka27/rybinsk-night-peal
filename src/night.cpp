#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& a) {
    Night b;
    int n = a.size(), k = -1;
    for(int i = 0; i < n; i++) {
        if(a[i].size() == 0) {
            b.fates.push_back(SheetFate::Empty);
            continue;
        }
        if(k != -1 && a[i] == a[k]) {
            b.fates.push_back(SheetFate::Repeat);
            continue;
        }
        k = i;
        Voice c = voiceOf(a[i]);
        if(!c.accepted) {
            b.fates.push_back(SheetFate::Unfit);
            continue;
        }
        b.played += c.peal;
        b.fates.push_back(SheetFate::Played);
    }
    return b;
}

}  // namespace rybinsk