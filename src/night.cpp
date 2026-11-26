#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night a;
    a.fates.reserve(parts.size());

    const Peal* b = nullptr;

    for (int i = 0; i < static_cast<int>(parts.size()); ++i) {
        if (parts[i].size() == 0) {
            a.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (b != nullptr && parts[i] == *b) {
            a.fates.push_back(SheetFate::Repeat);
            continue;
        }

        b = &parts[i];

        Voice c = voiceOf(parts[i]);
        if (!c.accepted) {
            a.fates.push_back(SheetFate::Unfit);
        } else {
            a.fates.push_back(SheetFate::Played);
            a.played += c.peal;
        }
    }

    return a;
}

}  // namespace rybinsk