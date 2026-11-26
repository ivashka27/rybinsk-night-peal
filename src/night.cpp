#include "rybinsk/night.hpp"

#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    Peal prev_part;
    result.fates.resize(parts.size());
    for (int i = 0; i < parts.size(); ++i) {

        if (parts[i].size()==0) {
            result.fates[i]=SheetFate::Empty;
            continue;
        }

        else if (i>=1 and prev_part==parts[i]) {
            result.fates[i]=SheetFate::Repeat;
            prev_part = parts[i];
            continue;
        }

        else if (voiceOf((parts[i])).accepted==false) {
            result.fates[i]=SheetFate::Unfit;
            prev_part = parts[i];

            continue;
        }
        result.fates[i]=SheetFate::Played;
        Voice voice = voiceOf(parts[i]);
        result.played += voice.peal;
        prev_part = parts[i];


    }
    return result;


}

}  // namespace rybinsk
