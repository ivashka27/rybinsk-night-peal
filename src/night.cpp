#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    Voice voice;
    Peal previous;
    bool has_previous = false;

    for (int i = 0; i < parts.size(); ++i) {
        if (parts[i].size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (!(has_previous)) {
            previous = parts[i];
            has_previous = true;
            voice = voiceOf(parts[i]);

            if (!(voice.accepted)) {
                night.fates.push_back(SheetFate::Unfit);
            } else {
                night.fates.push_back(SheetFate::Played);
                night.played += voice.peal;
            }
        } else {
            if (parts[i] == previous) {
                night.fates.push_back(SheetFate::Repeat);
            } else {
                previous = parts[i];
                voice = voiceOf(parts[i]);

                if (!(voice.accepted)) {
                    night.fates.push_back(SheetFate::Unfit);
                } else {
                    night.fates.push_back(SheetFate::Played);
                    night.played += voice.peal;
                }
            }
        }
    }
    return night;

}

}  // namespace rybinsk
