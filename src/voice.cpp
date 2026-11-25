#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    std::vector<Stroke> a;
    bool used[9] = {};

    for (int i = 0; i < sheet.size(); i++) {
        int b = sheet[i].bell;
        int t = sheet[i].beats;

        if (b < 0 || b > 8 || t <= 0) {
            return {false, Peal()};
        }

        if (b == 0) {
            if (t != 1) {
                return {false, Peal()};
            }

            a.push_back(sheet[i]);

            for (int j = 0; j < 9; j++) {
                used[j] = false;
            }
        } else {
            if (!used[b]) {
                a.push_back(sheet[i]);
                used[b] = true;
            }
        }
    }

    return {true, Peal(a.data(), static_cast<int>(a.size()))};
}

}