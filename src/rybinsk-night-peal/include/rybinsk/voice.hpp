#pragma once

#include "rybinsk/peal.hpp"

namespace rybinsk {

// Голос листка: то, что колокольня готова звонить.
// accepted == false — в листке есть удар, который портит его целиком.
// Тогда peal пустой. Исходный листок функция не меняет.
struct Voice {
    bool accepted;
    Peal peal;
};

[[nodiscard]] Voice voiceOf(const Peal& sheet);

}  // namespace rybinsk
