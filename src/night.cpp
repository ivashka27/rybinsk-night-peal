#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night result;
    bool haslast = false;
    Peal lastsheet;
    
    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            result.fates.push_back(SheetFate::Empty);
        } else if (haslast && sheet == lastsheet) {
            result.fates.push_back(SheetFate::Repeat);
        } else {
            haslast = true;
            lastsheet = sheet;
            Voice v = voiceOf(sheet);
            if (!v.accepted) {
                result.fates.push_back(SheetFate::Unfit);
            } else {
                result.fates.push_back(SheetFate::Played);
                result.played += v.peal;
            }
        }
    }
    return result;
}

}  // namespace rybinsk
