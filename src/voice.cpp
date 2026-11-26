#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    int n = sheet.size();
    if (n == 0) {
        return {true, Peal()};
    }

    bool a[9] = {false};
    int b = 0;

    for (int i = 0; i < n; ++i) {
        int c = sheet[i].bell;
        int d = sheet[i].beats;

        if (c == 0) {
            if (d != 1) {
                return {false, Peal()};
            }
            for (int j = 1; j <= 8; ++j) {
                a[j] = false;
            }
            ++b;
        } else if (c >= 1 && c <= 8 && d >= 1) {
            if (!a[c]) {
                a[c] = true;
                ++b;
            }
        } else {
            return {false, Peal()};
        }
    }

    Peal e(b);
    for (int j = 1; j <= 8; ++j) {
        a[j] = false;
    }

    int k = 0;
    for (int i = 0; i < n; ++i) {
        int c = sheet[i].bell;
        if (c == 0) {
            for (int j = 1; j <= 8; ++j) {
                a[j] = false;
            }
            e[k++] = sheet[i];
        } else if (!a[c]) {
            a[c] = true;
            e[k++] = sheet[i];
        }
    }

    return {true, e};
}

}  // namespace rybinsk