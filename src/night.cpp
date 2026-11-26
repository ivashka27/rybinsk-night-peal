#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"
namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    Peal last;
    bool hasLast = false;
    for (int i = 0; i < parts.size(); i++) {
        if (parts[i].size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (hasLast && parts[i] == last) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }
        Voice voice = voiceOf(parts[i]);
        if (voice.accepted == true) {
            night.fates.push_back(SheetFate::Played);
            night.played += voice.peal;
        } else {
            night.fates.push_back(SheetFate::Unfit);
        }
        last = parts[i];
        hasLast = true;
    }
    return night;
}

}  // namespace rybinsk
