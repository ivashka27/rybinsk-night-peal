#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {
namespace {

bool sameSheet(const Peal& a, const Peal& b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (int i = 0; i < a.size(); ++i) {
        if (a[i].bell != b[i].bell || a[i].beats != b[i].beats) {
            return false;
        }
    }

    return true;
}

}  // namespace

Night ringNight(const std::vector<Peal>& parts) {
    Night result{Peal(), {}};
    std::vector<Peal> seen;

    for (const Peal& part : parts) {
        if (part.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        bool repeated = false;

        for (const Peal& previous : seen) {
            if (sameSheet(part, previous)) {
                repeated = true;
                break;
            }
        }

        if (repeated) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        seen.push_back(part);

        Voice voice = voiceOf(part);

        if (!voice.accepted) {
            result.fates.push_back(SheetFate::Unfit);
            continue;
        }

        result.fates.push_back(SheetFate::Played);

        if (result.played.size() == 0) {
            result.played = voice.peal;
        } else {
            result.played = result.played + voice.peal;
        }
    }

    return result;
}

}  // namespace rybinsk