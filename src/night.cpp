#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"
namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    result.fates.reserve(parts.size());

    const Peal* previous = nullptr;

    for (const Peal& part : parts) {
        if (part.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
            continue;
        }

        if (previous != nullptr && part == *previous) {
            result.fates.push_back(SheetFate::Repeat);
            continue;
        }

        previous = &part;
        Voice voice = voiceOf(part);

        if (!voice.accepted) {
            result.fates.push_back(SheetFate::Unfit);
        } else {
            result.played += voice.peal;
            result.fates.push_back(SheetFate::Played);
        }
    }

    return result;
}

}  // namespace rybinsk
