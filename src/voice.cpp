#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    Voice voice;

    for (int i = 0; i < sheet.size(); ++i) {
        if ((sheet[i].bell >= 1 && sheet[i].bell <= 8) && sheet[i].beats >= 1) {
            continue;
        } else if (sheet[i].bell == 0 && sheet[i].beats == 1) {
            continue;
        } else {
            voice.accepted = false;
            return voice;
        }
    }

    voice.accepted = true;
    bool seen[9]{};
    int cnt = 0;

    for (int i = 0; i < sheet.size(); ++i) {
        if (sheet[i].bell == 0) {
            ++cnt;

            for (int j = 0; j < 9; ++j) {
                seen[j] = false;
            }
        } else if (!seen[sheet[i].bell]) {
            seen[sheet[i].bell] = true;
            ++cnt;
        }
    }

    voice.peal = Peal(cnt);

    for (int j = 0; j < 9; ++j) {
        seen[j] = false;
    }

    int out = 0;

    for (int i = 0; i < sheet.size(); ++i) {
        if (sheet[i].bell == 0) {
            voice.peal[out] = sheet[i];
            ++out;
            for (int j = 0; j < 9; ++j) {
                seen[j] = false;
            }
        } else if (!seen[sheet[i].bell]) {
            seen[sheet[i].bell] = true;
            voice.peal[out] = sheet[i];
            ++out;
        }
    }

    return voice;
}

}  // namespace rybinsk
