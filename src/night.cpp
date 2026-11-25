#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night ans;
    int last = -1;

    for (int i = 0; i < static_cast<int>(parts.size()); i++) {
        if (parts[i].size() == 0) {
            ans.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (last != -1 && parts[i] == parts[last]) {
            ans.fates.push_back(SheetFate::Repeat);
            continue;
        }

        last = i;

        Voice a = voiceOf(parts[i]);

        if (!a.accepted) {
            ans.fates.push_back(SheetFate::Unfit);
        } else {
            ans.played += a.peal;
            ans.fates.push_back(SheetFate::Played);
        }
    }

    return ans;
}

}