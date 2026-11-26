#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& other) {
    bool heard[9] = {};
    Voice result;
    result.accepted = true;
    Stroke* temp = new Stroke[other.size()]{};
    int pos = 0;
    for (int i=0; i < other.size(); i++){
        if(other[i].bell >= 1 and other[i].bell <= 8 and other[i].beats >= 1){
            if (heard[other[i].bell] == false){
                temp[pos] = other[i];
                pos+=1;
            }
            heard[other[i].bell] = true;

        }
        else if(other[i].bell == 0 and other[i].beats == 1){
            temp[pos] = other[i];
            pos+=1;
            for (int j=0;j < 9; j++){
                heard[j] = false;
            }

        }
        else{
            result.accepted = false;
            delete[] temp;
            temp = nullptr;
            return result;
        }

    }
    result.peal = Peal(temp, pos);
    delete[] temp;
    temp = nullptr;
    return result;
}
}  // namespace rybinsk
