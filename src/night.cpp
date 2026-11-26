#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"//без этого файла не работает voiceOf

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts){
    Night res;
    res.played=Peal();

    bool has_prev=false;
    Peal prev_sheet;

    for(int idx=0;idx<(int)parts.size();idx++){
        const Peal& cur=parts[idx];

        if(cur.size()==0){
            res.fates.push_back(SheetFate::Empty);
            continue;
        }

        if(has_prev and cur==prev_sheet){
            res.fates.push_back(SheetFate::Repeat);
            continue;
        }

        Voice voc=voiceOf(cur);
        if(not voc.accepted){
            res.fates.push_back(SheetFate::Unfit);
            prev_sheet=cur;
            has_prev=true;
            continue;
        }

        res.played+=voc.peal;
        res.fates.push_back(SheetFate::Played);
        prev_sheet=cur;
        has_prev=true;
    }

    return res;
}

}  // namespace rybinsk
