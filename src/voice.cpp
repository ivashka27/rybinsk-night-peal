#include "rybinsk/voice.hpp"
#include <vector>
namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    Voice v;
    v.accepted = true;
    std::vector<Stroke> s;
    bool bells[9] = {};
    int p = 0;
    for (int i = 0; i < sheet.size(); i++) {
        if (sheet[i].bell == 0) {
            if (sheet[i].beats == 1) {
                s.push_back(sheet[i]);
                p++;
                for (int j = 1; j <= 8; j++) {
                    bells[j] = false;
                }
            }else{
                v.accepted = false;
                return v;
            }
        } else {
            if (sheet[i].bell < 1 || sheet[i].bell > 8) {
                v.accepted = false;
                return v;
            }
            if (sheet[i].beats < 1) {
                v.accepted = false;
                return v;
            }
            if (bells[sheet[i].bell] == false) {
                s.push_back(sheet[i]);
                p++;
                bells[sheet[i].bell] = true;
            }
        }
    }
    Peal result(s.data(), p);
    v.peal = result;
    return v;
}

}  // namespace rybinsk
