#include "rybinsk/voice.hpp"
#include "rybinsk/peal.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& peal) {
    Voice result;
    result.accepted = false;

    for (int i = 0; i < peal.size(); i++) {
        Stroke s = peal[i];
        if (s.bell >= 1 && s.bell <= 8) {
            if (s.beats < 1) return result;
        } else if (s.bell == 0) {
            if (s.beats != 1) return result;
        } else {
            return result;
        }
    }

    int maxSize = peal.size();
    Stroke* buf = nullptr;
    if (maxSize > 0) buf = new Stroke[maxSize];

    bool used[9];
    for (int b = 0; b < 9; b++) used[b] = false;

    int k = 0;
    for (int i = 0; i < peal.size(); i++) {
        Stroke s = peal[i];
        if (s.bell == 0) {
            buf[k++] = s;
            for (int b = 0; b < 9; b++) used[b] = false;
        } else {
            if (used[s.bell] == false) {
                buf[k++] = s;
                used[s.bell] = true;
            }
        }
    }

    result.accepted = true;
    result.peal = Peal(buf, k);
    delete[] buf;
    return result;
}

} // namespace rybinsk