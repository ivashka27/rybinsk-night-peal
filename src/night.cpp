#include "rybinsk/night.hpp"

#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    const Peal* last = nullptr;
    for (const Peal& part : parts) {
        if (part.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
        } else if (last != nullptr && part == *last) {
            night.fates.push_back(SheetFate::Repeat);
        } else {
            last = &part;
            Voice v = voiceOf(part);
            if (!v.accepted) {
                night.fates.push_back(SheetFate::Unfit);
            } else {
                night.played += v.peal;
                night.fates.push_back(SheetFate::Played);
            }
        }
    }
    return night;
}

}  // namespace rybinsk