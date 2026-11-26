#include "rybinsk/voice.hpp"
#include <vector>
namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    
    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];
        const bool rest = stroke.bell == 0 && stroke.beats == 1;
        const bool bell = stroke.bell >= 1 && stroke.bell <= 8
                       && stroke.beats >= 1;
        if (!rest && !bell) {
            return Voice{false, Peal()};
        }
    }

    std::vector<Stroke> cleaned;
    bool heard[9] = {};

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];

        if (stroke.bell == 0) {
            cleaned.push_back(stroke);
            for (int bell = 1; bell <= 8; ++bell) {
                heard[bell] = false;
            }
        } else if (!heard[stroke.bell]) {
            cleaned.push_back(stroke);
            heard[stroke.bell] = true;
        }
    }

    return Voice{true, Peal(cleaned.data(), static_cast<int>(cleaned.size()))};
}

}  // namespace rybinsk
