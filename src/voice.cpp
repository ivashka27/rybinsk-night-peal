#include "rybinsk/voice.hpp"

#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& p) {
    Voice result; // Создаем объект, чтобы заполнить его явно
    
    for (int i = 0; i < p.size(); ++i) {
        const Stroke& s = p[i];
        if (s.bell == 0) {
            if (s.beats != 1) {
                result.peal = Peal();
                result.accepted = false;
                return result;
            }
        } else if (s.bell >= 1 && s.bell <= 8) {
            if (s.beats <= 0) {
                result.peal = Peal();
                result.accepted = false;
                return result;
            }
        } else {
            result.peal = Peal();
            result.accepted = false;
            return result;
        }
    }

    std::vector<Stroke> res;
    bool memory[9] = {false};

    for (int i = 0; i < p.size(); ++i) {
        const Stroke& s = p[i];
        if (s.bell == 0) {
            for (int b = 1; b <= 8; ++b) {
                memory[b] = false;
            }
            res.push_back(s);
        } else {
            if (!memory[s.bell]) {
                memory[s.bell] = true;
                res.push_back(s);
            }
        }
    }

    result.peal = Peal(res.data(), static_cast<int>(res.size()));
    result.accepted = true;
    return result;
}

}  // namespace rybinsk