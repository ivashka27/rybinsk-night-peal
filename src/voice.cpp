#include "rybinsk/voice.hpp"

namespace rybinsk {
Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); i++) {
        int bell = sheet[i].bell;
        int beats = sheet[i].beats;
        if (bell == 0) {
            if (beats != 1) {
                return {false, Peal()};
            }
        } else {
            if (bell < 1 || bell > 8 || beats < 1) {
                return {false, Peal()};
            }
        }
    }
    bool used[9] = {};
    Stroke* result = new Stroke[sheet.size()];
    int result_count = 0;
    for (int i = 0; i < sheet.size(); i++) {
        Stroke current = sheet[i];
        if (current.bell == 0) {
            result[result_count] = current;
            result_count++;
            for (int j = 1; j <= 8; j++) {
                used[j] = false;
            }
        } else {
            if (!used[current.bell]) {
                result[result_count] = current;
                result_count++;
                used[current.bell] = true;
            }
        }
    }

    Peal answer(result, result_count);
    delete[] result;
    return {true, static_cast<Peal&&>(answer)};
}
}