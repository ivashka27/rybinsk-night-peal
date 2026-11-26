#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;

    Peal previous;
    bool hasPrevious = false;

    for (const Peal& part : parts) {
        // Пустой листок просто перелистываем
        // Он не меняет предыдущий непустой листок
        if (part.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        // Проверяем, не встречался ли такой листок раньше
        if (hasPrevious && part == previous) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        // Запоминаем листок, даже если он окажется непригодным
        previous = part;
        hasPrevious = true;

        // Проверяем, можно ли исполнить листок
        Voice voice = voiceOf(part);

        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }

        // Добавляем пригодную партию к общей ночи
        night.played += voice.peal;
        night.fates.push_back(SheetFate::Played);
    }

    return night;
}

}  // namespace rybinsk
