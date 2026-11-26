#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    bool used[9]{};
    std::vector<Stroke> result;

    for (int i = 0; i < sheet.size(); i++) {
        int bell = sheet[i].bell;
        int beats = sheet[i].beats;

        if (bell < 0 || bell > 8 || beats <= 0) {
            return Voice{false, Peal()};
        }

        if (bell == 0 && beats != 1) {
            return Voice{false, Peal()};
        }

        if (bell == 0) {
            result.push_back(sheet[i]);

            for (int j = 1; j <= 8; j++) {
                used[j] = false;
            }
        }
        else if (used[bell] == false) {
            result.push_back(sheet[i]);
            used[bell] = true;
        }
    }

    return Voice{true, Peal(result.data(), result.size())};
}

} //namespace rybinsk