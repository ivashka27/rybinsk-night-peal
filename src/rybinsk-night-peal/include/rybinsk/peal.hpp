#pragma once

#include "rybinsk/stroke.hpp"

#include <stdexcept>

namespace rybinsk {

class PealError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Партия ударов. Память под удары принадлежит объекту.
// Публичные сигнатуры не меняйте: тесты собираются против них.
// Поля ниже тоже оставьте как есть. Свои приватные методы добавить можно.
class Peal {
public:
    Peal();

    // Ровно count нулевых ударов. count == 0 — пустая партия, без массива.
    // count < 0 — PealError, память не выделяется.
    explicit Peal(int count);

    // Копия count ударов из массива.
    // count == 0 — пустая партия, strokes может быть nullptr.
    // count < 0 или (strokes == nullptr && count > 0) — PealError.
    Peal(const Stroke* strokes, int count);

    Peal(const Peal& other);
    Peal(Peal&& other) noexcept;
    ~Peal();

    Peal& operator=(const Peal& other);
    Peal& operator=(Peal&& other) noexcept;

    [[nodiscard]] int size() const;

    Stroke& operator[](int index);
    const Stroke& operator[](int index) const;

    // Сложить два листка вперебор. Слагаемые не меняются. Правило — в README.
    Peal operator+(const Peal& other) const;

    // Заменить себя на *this + other. Вернуть *this.
    Peal& operator+=(const Peal& other);

    [[nodiscard]] bool operator==(const Peal& other) const;
    [[nodiscard]] bool operator!=(const Peal& other) const;

    // Сумма beats. Пустая партия — 0.
    [[nodiscard]] explicit operator int() const;

private:
    Stroke* strokes_ = nullptr;
    int count_ = 0;
};

}  // namespace rybinsk
