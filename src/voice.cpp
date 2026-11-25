#include "rybinsk/voice.hpp"
#include <utility>
namespace rybinsk {

Voice voiceOf(const Peal& peal) {
    int count = peal.size();
    if (count == 0) {
        return {true, Peal()};
    }

    for (int i = 0; i < count; ++i) {
        const Stroke& s = peal[i];
        bool isBell = (s.bell >= 1 && s.bell <= 8 && s.beats >= 1);
        bool isPause = (s.bell == 0 && s.beats == 1);
        if (!isBell && !isPause) {
            return {false, Peal()};
        }
    }

    Peal buffer(count);
    int pos = 0;
    bool seen[9]{};

    for (int i = 0; i < count; ++i) {
        const Stroke& s = peal[i];
        if (s.bell == 0) {
            buffer[pos++] = s;
            for (int b = 1; b <= 8; ++b) {
                seen[b] = false;
            }
        } else if (!seen[s.bell]) {
            seen[s.bell] = true;
            buffer[pos++] = s;
        }
    }

    Peal result(pos);
    for (int i = 0; i < pos; ++i) {
        result[i] = buffer[i];
    }

    return {true, std::move(result)};
}
}  // namespace rybinsk
