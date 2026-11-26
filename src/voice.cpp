#include "rybinsk/voice.hpp"
#include <array>
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& listok) {
    std::vector<Stroke> chistie_udary;
    chistie_udary.reserve(static_cast<std::size_t>(listok.size()));

    std::array<bool, 9> uzhe_zvuchal{};

    for (int i = 0; i < listok.size(); ++i) {
        const Stroke& udar = listok[i];

        if (udar.bell == 0) {
            if (udar.beats != 1) {
                    return {false, Peal()};
                }
            uzhe_zvuchal.fill(false);
            chistie_udary.push_back(udar);
            } else if (udar.bell >= 1 && udar.bell <= 8) {
                if (udar.beats < 1) {
                    return {false, Peal()};
                }
                if (!uzhe_zvuchal[udar.bell]){
                    uzhe_zvuchal[udar.bell] = true;
                    chistie_udary.push_back(udar);
                }
            } else {
                return {false, Peal()};
            }
        }
        return {true, Peal(chistie_udary.data(), static_cast<int>(chistie_udary.size()))};
    }

}  // namespace rybinsk
