#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"
namespace rybinsk {

Night ringNight(const std::vector<Peal>& peals) {
    Night result;
    int last = -1;
    int range = peals.size();
    for (int i=0; i < range;i++){
            if (peals[i].size() == 0){
                result.fates.push_back(SheetFate::Empty);
                continue;
            }
            else if(last != -1 and peals[i] == peals[last]){
                result.fates.push_back(SheetFate::Repeat);
                last = i;
                continue;
            }
            else if (voiceOf(peals[i]).accepted == false){
                result.fates.push_back(SheetFate::Unfit);
                last = i;
            }
            else{
                result.fates.push_back(SheetFate::Played);
                result.played = result.played + voiceOf(peals[i]).peal;
                last = i;
            }
    }
    return result;
}

}  // namespace rybinsk
