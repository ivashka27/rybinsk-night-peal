#pragma once

#include "rybinsk/peal.hpp"

#include <vector>

namespace rybinsk {

// Пометка одного листка из стопки, в том же порядке, в каком листки пришли.
enum class SheetFate {
    Played,  // голос сложили с ночью
    Empty,   // пустой лист перелистнули
    Repeat,  // тот же листок, что уже рассматривали
    Unfit,   // плохой удар, листок не звонили
};

struct Night {
    Peal played;
    std::vector<SheetFate> fates;
};

// Разобрать стопку. played — одна партия ночи. fates[i] — пометка parts[i].
// Список на входе не меняется. Подробности — в README.
[[nodiscard]] Night ringNight(const std::vector<Peal>& parts);

}  // namespace rybinsk
