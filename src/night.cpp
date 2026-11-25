#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& stack) {
    Night night;
    night.played = Peal();
    bool haveLastSheet = false;
    Peal lastSheat;

    for(int i = 0; i < stack.size(); i++){
        Voice voice = voiceOf(stack[i]);
        if(!haveLastSheet){
            if(!voice.accepted){
                night.fates.push_back(SheetFate::Unfit);
                lastSheat = stack[i];
                haveLastSheet = true;
            }
            else if(voice.peal.size() == 0){
                night.fates.push_back(SheetFate::Empty);
            }
            else{
                night.played += voice.peal;
                night.fates.push_back(SheetFate::Played);
                lastSheat = stack[i];
                haveLastSheet = true;
            }
            continue;
        }

        if(lastSheat == stack[i]){
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        if(!voice.accepted){
            night.fates.push_back(SheetFate::Unfit);
            lastSheat = stack[i];
            continue;
        }
        else{
            if(voice.peal.size() == 0){
                night.fates.push_back(SheetFate::Empty);
            }
            else{
                night.played += voice.peal;
                night.fates.push_back(SheetFate::Played);
                lastSheat = stack[i];
                haveLastSheet = true;
            }
            continue;
        }
    }

    return night;
}

}  // namespace rybinsk
