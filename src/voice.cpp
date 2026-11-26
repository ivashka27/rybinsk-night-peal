#include "rybinsk/voice.hpp"

#include <set>

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    bool accepted = true;
    for (int i = 0; i < sheet.size(); i++) {
        const auto [bell, beats] = sheet[i];

        if (!(1 <= bell && bell <= 8 && beats >= 1) && !(bell == 0 && beats == 1)) {
            accepted = false;
        }
    }

    if (!accepted) {
        return {accepted, Peal()};
    }

    int valid_count = 0;

    std::set<int> bell_history;
    for (int i = 0; i < sheet.size(); i++) {
        auto [bell, beats] = sheet[i];

        if (bell != 0 && bell_history.find(bell) == bell_history.end()) {
            bell_history.insert(bell);
            valid_count++;
        }

        if (bell == 0) {
            bell_history = std::set<int>();
            valid_count++;
        }
    }

    Peal tmp = Peal(valid_count);
    int tmp_ptr = 0;

    bell_history = std::set<int>();
    for (int i = 0; i < sheet.size(); i++) {
        auto [bell, beats] = sheet[i];

        if (bell != 0 && bell_history.find(bell) == bell_history.end()) {
            bell_history.insert(bell);
            tmp[tmp_ptr++] = sheet[i];
        }

        if (bell == 0) {
            bell_history = std::set<int>();
            tmp[tmp_ptr++] = sheet[i];
        }
    }

    return {accepted, tmp};
}

}  // namespace rybinsk
