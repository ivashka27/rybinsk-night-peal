#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"
namespace rybinsk {
Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    bool has_prev = false;
    Peal prev;
    for (const Peal& sheet: parts) {
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (has_prev && sheet == prev) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }
        prev = sheet;
        has_prev = true;
        Voice voice = voiceOf(sheet);
        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }
        night.fates.push_back(SheetFate::Played);
        night.played += voice.peal;
    }
    return night;
}
}  // namespace rybinsk