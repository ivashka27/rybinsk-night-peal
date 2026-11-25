#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    Peal previous;

    for (int i = 0; i < static_cast<int>(parts.size()); i++) {
        const Peal& sheet = parts[i];

        if (sheet.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (sheet == previous) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        previous = sheet;

        Voice voice = voiceOf(sheet);

        if (voice.accepted == false) {
            result.fates.push_back(SheetFate::Unfit);
        }
        else {
            result.played += voice.peal;
            result.fates.push_back(SheetFate::Played);
        }
    }

    return result;
}

}  // namespace rybinsk
