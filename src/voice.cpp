#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& blueprint) {
    Voice result;
    result.accepted = true;

    for (int i = 0; i < blueprint.size(); ++i) {
        int bell = blueprint[i].bell;
        int beats = blueprint[i].beats;
        if (!((1 <= bell and bell <= 8 and beats >= 1) or (beats == 1 and bell==0))){
            result.accepted = false;
            result.peal = Peal();
            return result;
        }
    }

    int* already_seen = new int[blueprint.size()];
    Stroke* will_be_strokes = new Stroke[blueprint.size()]();
    int already_seen_size = 0;
    int will_be_peal_len = 0;
    int next_stroke = 0;
    for (int i = 0; i < blueprint.size(); ++i) {
        bool break_flag = false;
        int bell = blueprint[i].bell;
        int beats = blueprint[i].beats;

        if (bell==0 and beats==1) {
            will_be_strokes[next_stroke]=blueprint[i];
            ++next_stroke;
            ++will_be_peal_len;
            already_seen_size=0;
            continue;
        }

        for (int ii = 0; ii < already_seen_size; ++ii) {
            if (blueprint[i].bell == already_seen[ii]) {
                break_flag = true;
                break;
            }
        }
        if (break_flag==true) continue;
        will_be_strokes[next_stroke]=blueprint[i];
        already_seen[already_seen_size] = blueprint[i].bell;
        already_seen_size++;
        ++next_stroke;
       ++will_be_peal_len;

    }

    result.peal = Peal(will_be_peal_len);
    for (int i = 0; i < will_be_peal_len; ++i) {
        result.peal[i] = will_be_strokes[i];
    }
    delete[] already_seen;
    delete[] will_be_strokes;
    return result;
}

}  // namespace rybinsk
