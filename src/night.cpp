#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    std::vector<SheetFate> fates(parts.size());
    Peal played = Peal();

    Peal last = Peal();
    for (size_t i = 0; i < parts.size(); i++) {
        auto [accepted, peal] = voiceOf(parts[i]);

        if (parts[i].size() == 0) {
            fates[i] = SheetFate::Empty;
        }
        else if (parts[i] == last) {
            fates[i] = SheetFate::Repeat;
        }
        else if (!accepted) {
            fates[i] = SheetFate::Unfit;
            last = parts[i];
        }
        else {
            fates[i] = SheetFate::Played;
            played += peal;
            last = parts[i];
        }
    }

    return {played, fates};
}

}  // namespace rybinsk
