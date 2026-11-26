#include "rybinsk/voice.hpp"

namespace rybinsk {

Voice voiceOf(const Peal& sheet) {
    Stroke* strokes = new Stroke[sheet.size()];
    int count = 0;

    bool used[9] = {};

    for (int i = 0; i < sheet.size(); i++) {
        Stroke stroke = sheet[i];

        // Проверяем количество долей
        if (stroke.beats < 1) {
            delete[] strokes;
            return {false, Peal()};
        }

        if (stroke.bell == 0) {
            // Пауза должна длиться одну долю
            if (stroke.beats != 1) {
                delete[] strokes;
                return {false, Peal()};
            }

            strokes[count] = stroke;
            count++;

            // Пауза сбрасывает историю колоколов
            for (int j = 1; j <= 8; j++) {
                used[j] = false;
            }
        } else {
            // Проверяем номер колокола
            if (stroke.bell < 1 || stroke.bell > 8) {
                delete[] strokes;
                return {false, Peal()};
            }

            // Добавляем только первое появление колокола
            if (!used[stroke.bell]) {
                strokes[count] = stroke;
                count++;

                used[stroke.bell] = true;
            }
        }
    }

    Peal result(strokes, count);
    delete[] strokes;

    return {true, result};
}

}  // namespace rybinsk
