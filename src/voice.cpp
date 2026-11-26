#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); i++) {
        int bell = sheet[i].bell;
        int beats = sheet[i].beats;

        if (bell < 0 || bell > 8 || beats < 1) {
            return {false, Peal()};
        }

        if (bell == 0 && beats != 1) {
            return {false, Peal()};
        }
    }

    bool seen[9] = {};
    Peal result(sheet.size());
    int count = 0;

    for (int i = 0; i < sheet.size(); i++) {
        int bell = sheet[i].bell;

        if (bell == 0) {
            result[count] = sheet[i];
            count++;

            for (int j = 0; j < 9; j++) {
                seen[j] = false;
            }
        } else if (seen[bell] == false) {
            result[count] = sheet[i];
            count++;
            seen[bell] = true;
        }
    }

    Peal answer(count);

    for (int i = 0; i < count; i++) {
        answer[i] = result[i];
    }

    return {true, answer};
}

}  // namespace rybinsk
