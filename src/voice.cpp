
#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    bool seen[9] = {};

    Peal result(sheet.size());
    int resultSize = 0;

    for (int i = 0; i < sheet.size(); ++i) {
        const Stroke& stroke = sheet[i];

        if (stroke.bell == 0 && stroke.beats == 1) {
            for (int j = 1; j <= 8; ++j) {
                seen[j] = false;
            }

            result[resultSize++] = stroke;
            continue;
        }

        if (stroke.bell < 1 || stroke.bell > 8 ||
            stroke.beats < 1) {
            return Voice{false, Peal()};
        }

        if (!seen[stroke.bell]) {
            result[resultSize++] = stroke;
            seen[stroke.bell] = true;
        }
    }

    Peal filtered(resultSize);

    for (int i = 0; i < resultSize; ++i) {
        filtered[i] = result[i];
    }

    return Voice{true, filtered};
}

}  // namespace rybinsk
