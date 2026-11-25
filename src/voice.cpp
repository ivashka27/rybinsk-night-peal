#include "rybinsk/voice.hpp"
#include "vector"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    for (int i = 0; i < sheet.size(); i++) {
        if (sheet[i].beats <= 0 || sheet[i].bell < 0 || sheet[i].bell > 8 ||
        (sheet[i].bell == 0 && sheet[i].beats != 1)) {
            return Voice{false, Peal()};
        }
    }
    
    std::vector<Stroke> result;
    bool seen[9] = {false};

    for (int i = 0; i < sheet.size(); i++) {
        if (sheet[i].bell == 0 && sheet[i].beats == 1) {
            for (int b = 1; b <= 8; b++) {
                seen[b] = false;
            }
            result.push_back(sheet[i]);
        } else if (!seen[sheet[i].bell]){
            seen[sheet[i].bell] = true;
            result.push_back(sheet[i]);
        }
    }

    return Voice{true, Peal(result.data(), static_cast<int>(result.size()))};
}

}  // namespace rybinsk
