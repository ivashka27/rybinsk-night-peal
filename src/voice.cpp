#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& a) {
    int n = a.size(), k = 0;
    bool b[9]{};
    for(int i = 0; i < n; i++) {
        int x = a[i].bell, y = a[i].beats;
        if(x < 0 || x > 8 || y <= 0 || (x == 0 && y != 1)) {
            return {false, Peal()};
        }
        if(x == 0) {
            k++;
            for(int j = 0; j < 9; j++) {
                b[j] = false;
            }
        } else if(!b[x]) {
            k++;
            b[x] = true;
        }
    }
    Peal c(k);
    k = 0;
    for(int i = 0; i < 9; i++) {
        b[i] = false;
    }
    for(int i = 0; i < n; i++) {
        int x = a[i].bell;
        if(x == 0) {
            c[k] = a[i];
            k++;
            for(int j = 0; j < 9; j++) {
                b[j] = false;
            }
        } else if(!b[x]) {
            c[k] = a[i];
            k++;
            b[x] = true;
        }
    }
    return {true, c};
}

}  // namespace rybinsk