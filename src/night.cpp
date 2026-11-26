#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    int previous = -1;

    for (int i = 0; i < parts.size(); i++) {

        if (parts[i].size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (previous != -1 && parts[i] == parts[previous]) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        previous = i;

        Voice voice = voiceOf(parts[i]);

        if (voice.accepted == false) {
            night.fates.push_back(SheetFate::Unfit);
        }
        else {
            night.played += voice.peal;
            night.fates.push_back(SheetFate::Played);
        }
    }

    return night;
}

}
