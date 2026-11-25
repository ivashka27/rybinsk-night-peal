#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk {

Voice voiceOf(const Peal& peal) {
    std::vector<Stroke> res;
    for (int i = 0; i < peal.size(); ++i){
        int bell = peal[i].bell;
        int beats = peal[i].beats;
        if (bell == 0 && beats == 1){
            continue;
        }
        if (bell < 1 || bell > 8 || beats < 1){
            return {false, Peal()};
        }
    }
    bool cout[9] = {};
    for (int i = 0; i < peal.size(); ++i){
        int bell = peal[i].bell;
        if (bell == 0){
            res.push_back(peal[i]);
            for (int j = 0; j<=8; ++j){
                cout[j] = false;
            }
            continue;
        }
        if (!cout[bell]){
            res.push_back(peal[i]);
            cout[bell] = true;
        }
    }
    Peal ans(res.data(), static_cast<int>(res.size()));
    return {true, std::move(ans)};

}

} 