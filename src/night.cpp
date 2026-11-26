#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"
namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    int lastIndex = -1;

    for (int i = 0; i < (int)parts.size(); ++i) {
        if (parts[i].size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (lastIndex != -1 && parts[i] == parts[lastIndex]) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }
        lastIndex = i;
        Voice v = voiceOf(parts[i]);
        if (!v.accepted) {
            night.fates.push_back(SheetFate::Unfit);
        } else {
            night.played += v.peal;
            night.fates.push_back(SheetFate::Played);
        }
    }
    return night;
}

}  // namespace rybinsk
