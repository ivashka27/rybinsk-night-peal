#include "rybinsk/peal.hpp"

#include <algorithm>
#include <utility>

namespace rybinsk {
namespace {

// Проверка длины и указателя. Вызывается до любого выделения памяти.
void checkArguments(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("Peal: отрицательное число ударов");
    }
    if (strokes == nullptr && count > 0) {
        throw PealError("Peal: пустой указатель при ненулевом числе ударов");
    }
}

// Новый массив из count нулевых ударов. count == 0 — nullptr.
Stroke* allocateZeroed(int count) {
    if (count == 0) {
        return nullptr;
    }
    return new Stroke[static_cast<std::size_t>(count)]();
}

// Новый массив — копия count ударов из from. count == 0 — nullptr.
Stroke* cloneStrokes(const Stroke* from, int count) {
    Stroke* result = allocateZeroed(count);
    if (count > 0) {
        std::copy(from, from + count, result);
    }
    return result;
}

}  // namespace

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Peal: отрицательное число ударов");
    }
    strokes_ = allocateZeroed(count);
    count_ = count;
}

Peal::Peal(const Stroke* strokes, int count) {
    checkArguments(strokes, count);
    strokes_ = cloneStrokes(strokes, count);
    count_ = count;
}

Peal::Peal(const Peal& other)
    : strokes_(cloneStrokes(other.strokes_, other.count_)), count_(other.count_) {}

Peal::Peal(Peal&& other) noexcept
    : strokes_(std::exchange(other.strokes_, nullptr)),
      count_(std::exchange(other.count_, 0)) {}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other) {
        return *this;
    }
    // Сначала копия: если выделение не удастся, *this останется прежним.
    Stroke* fresh = cloneStrokes(other.strokes_, other.count_);
    delete[] strokes_;
    strokes_ = fresh;
    count_ = other.count_;
    return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] strokes_;
    strokes_ = std::exchange(other.strokes_, nullptr);
    count_ = std::exchange(other.count_, 0);
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int index) {
    if (index < 0 || index >= count_) {
        throw PealError("Peal: номер удара вне листка");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Peal: номер удара вне листка");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int common = std::min(count_, other.count_);

    // Сколько ударов получится: пара с разными номерами даёт два удара,
    // с одинаковыми — один. Хвост длинного листка дописывается целиком.
    int total = std::max(count_, other.count_);
    for (int i = 0; i < common; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++total;
        }
    }

    Peal result(total);
    int out = 0;
    for (int i = 0; i < common; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell < b.bell) {
            result.strokes_[out++] = a;
            result.strokes_[out++] = b;
        } else if (b.bell < a.bell) {
            result.strokes_[out++] = b;
            result.strokes_[out++] = a;
        } else {
            result.strokes_[out++] = (a.beats >= b.beats) ? a : b;
        }
    }

    const Peal& longer = (count_ >= other.count_) ? *this : other;
    for (int i = common; i < longer.count_; ++i) {
        result.strokes_[out++] = longer.strokes_[i];
    }
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    // Новый листок считается, пока старый цел (важно для a += a).
    Peal sum = *this + other;
    *this = std::move(sum);
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) {
        return false;
    }
    for (int i = 0; i < count_; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell ||
            strokes_[i].beats != other.strokes_[i].beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return !(*this == other);
}

Peal::operator int() const {
    int sum = 0;
    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }
    return sum;
}

}  // namespace rybinsk
