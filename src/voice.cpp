#include "rybinsk/voice.hpp"
#include "map"
#include "algorithm"
#include "vector"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    Voice voice;
    voice.peal = Peal();
    voice.accepted = false;

    std::vector<Stroke> stroke;
    std::map<int, bool> durationOfBeats;

    for(int i = 1; i <= 8; i++){
        durationOfBeats[i] = false;
    }


    if(sheet.size() == 0){
        voice.accepted = true;
        voice.peal = sheet;
        
        return voice;
    }

    for(int i = 0; i < sheet.size(); i++){
        if(sheet[i].bell == 0){
            if(sheet[i].beats == 1){
                for(int i = 1; i <= 8; i ++){
                    durationOfBeats[i] = false;
                }
                stroke.push_back(sheet[i]);
            }
            else{
                return voice;
            }
        }
        else{
            if(1 <= sheet[i].bell && sheet[i].bell <= 8 && sheet[i].beats >= 1){
                if(durationOfBeats[sheet[i].bell]){
                    continue;
                }
                else{
                    durationOfBeats[sheet[i].bell] = true;
                    stroke.push_back(sheet[i]);
                }
            }
            else{
                return voice;
            }
        }
    }

    if(stroke.size() > 0){
        Stroke *strokePtr = &stroke[0];
        int count = stroke.size();
        voice.peal = Peal(strokePtr, count);
    }

    voice.accepted = true;
    return voice;
}

}  // namespace rybinsk
