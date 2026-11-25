#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    Peal prev;
    bool hasPrev = false;
    for (const Peal& part : parts){
        if (part.size() == 0){
            night.fates.push_back(SheetFate::Empty);
            continue;
        }
        if (hasPrev && part == prev){
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }
        prev = part;
        hasPrev = true;

        Voice voice = voiceOf(part);
        if (!voice.accepted){
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }
        night.played += voice.peal;
        night.fates.push_back(SheetFate::Played);


    }
    return night;
}

}  
