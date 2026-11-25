
#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;

    Peal previous;
    bool hasPrevious = false;

    for (const Peal& sheet : parts) {

        // Пустой листок просто перелистываем
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        // Проверяем повтор до проверки голоса
        if (hasPrevious && sheet == previous) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        // Проверяем, подходит ли листок для колокольни
        Voice voice = voiceOf(sheet);

        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
        } else {
            night.fates.push_back(SheetFate::Played);

            // Добавляем сыгранную партию в ночь
            night.played += voice.peal;
        }

        // Запоминаем последний рассмотренный непустой листок
        previous = sheet;
        hasPrevious = true;
    }

    return night;
}

} // namespace rybinsk
