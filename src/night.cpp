#include "rybinsk/night.hpp"

#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
    Night night;
    night.fates.reserve(parts.size());

    // Последний рассмотренный (не пустой) листок. Список на входе const
    // и не меняется, поэтому указатель на его элемент безопасен.
    const Peal* last = nullptr;

    for (const Peal& sheet : parts) {
        if (sheet.size() == 0) {
            night.fates.push_back(SheetFate::Empty);
            continue;
        }

        // Повтор проверяется раньше брака и сравнивает листок как он есть.
        if (last != nullptr && sheet == *last) {
            night.fates.push_back(SheetFate::Repeat);
            continue;
        }

        last = &sheet;

        Voice voice = voiceOf(sheet);
        if (!voice.accepted) {
            night.fates.push_back(SheetFate::Unfit);
            continue;
        }

        night.played += voice.peal;
        night.fates.push_back(SheetFate::Played);
    }

    return night;
}

}  // namespace rybinsk
