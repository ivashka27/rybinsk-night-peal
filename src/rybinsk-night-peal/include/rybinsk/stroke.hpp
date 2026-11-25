#pragma once

namespace rybinsk {

// Один удар. Это агрегат: конструкторов нет, тесты собирают его скобками.
// bell — номер колокола, beats — сколько долей он звучит.
// Сам листок номер не проверяет: хранить можно и 0, и 9.
// Годится ли удар для колокольни, решает voiceOf, не Peal.
struct Stroke {
    int bell;
    int beats;
};

}  // namespace rybinsk
