#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp" 
#include <utility>

namespace rybinsk {

Night ringNight(const std::vector<Peal>& stopka) {
    Peal noch_partiya;
    std::vector<SheetFate> pometki;
    pometki.reserve(stopka.size());

    const Peal* proshliy_listok = nullptr;
    for (const Peal& listok : stopka) {
        if (listok.size() == 0) {
            pometki.push_back(SheetFate::Empty);
            continue;
        }
        if (proshliy_listok != nullptr && listok == *proshliy_listok) {
            pometki.push_back(SheetFate::Repeat);
            continue;
        }

        proshliy_listok = &listok;

        Voice v = voiceOf(listok);
        if (!v.accepted) {
            pometki.push_back(SheetFate::Unfit);
        } else {
            noch_partiya += v.peal;
            pometki.push_back(SheetFate::Played);
        }
    }

    return {std::move(noch_partiya), std::move(pometki)};
}

}  // namespace rybinsk
