#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& stack) {
    Night report;
    Peal played;

    bool has_last = false;
    Peal last_considered;

    for (const Peal& current : stack) {
        if (current.size() == 0) {
            report.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (has_last && current == last_considered) {
            report.fates.push_back(SheetFate::Repeat);
            continue;
        }

        Voice vr = voiceOf(current);
        if (!vr.accepted) {
            report.fates.push_back(SheetFate::Unfit);
            last_considered = current;
            has_last = true;
            continue;
        }

        report.fates.push_back(SheetFate::Played);
        played += vr.peal;
        last_considered = current;
        has_last = true;
    }

    report.played = played;
    return report;
}

}  // namespace rybinsk