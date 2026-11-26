#include "rybinsk/voice.hpp"
#include <vector>
namespace rybinsk {

    Voice voiceOf(const Peal& sheet) {
        std::vector<Stroke> result;
        bool seen[9] = {};

        for (int i = 0; i < sheet.size(); i++) {
            Stroke stroke = sheet[i];

            if (stroke.bell == 0) {
                if (stroke.beats != 1) {
                    return {false, Peal()};
                }
                result.push_back(stroke);
                for (int j = 0; j < 9; j++) {
                    seen[j] = false;
                }
            } else {
                if (stroke.bell < 1 || stroke.bell > 8 ||
                    stroke.beats < 1) {
                    return {false, Peal()};
                    }
                if (!seen[stroke.bell]) {
                    result.push_back(stroke);
                    seen[stroke.bell] = true;
                }
            }
        }
        return {
            true,
            Peal(result.data(), static_cast<int>(result.size()))
        };
    }   
}// namespace rybinsk