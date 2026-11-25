#include "rybinsk/peal.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>

namespace rybinsk {
namespace {

void checkIndex(int index, int count) {
    if (index < 0 || index >= count) {
        throw PealError("Нет удара с таким индексом");
    }
}

}  // namespace

Peal::Peal() {}

Peal::Peal(int count) : count_(count) {
    if (count < 0) {
        throw PealError("Отрицательная длина партии");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
    }
}

Peal::Peal(const Stroke* strokes, int count) : count_(count) {
    if (count < 0 || (count > 0 && strokes == nullptr)) {
        throw PealError("Некорректный массив ударов");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        std::copy_n(strokes, count, strokes_);
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
Peal::Peal(Peal&& other) noexcept : strokes_(other.strokes_), count_(other.count_) {
    other.strokes_ = nullptr;
    other.count_ = 0;
}

Peal::~Peal() {
    // Освободите strokes_. delete[] nullptr вызывать можно.
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this != &other) {
        *this = Peal(other);
    }
    return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
    if (this != &other) {
        delete[] strokes_;
        strokes_ = other.strokes_;
        count_ = other.count_;
        other.strokes_ = nullptr;
        other.count_ = 0;
    }
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int index) {
    checkIndex(index, count_);
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    checkIndex(index, count_);
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int pairs = std::min(count_, other.count_);
    const Peal& longer = count_ >= other.count_ ? *this : other;
    int extra = 0;
    for (int i = 0; i < pairs; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++extra;
        }
    }
    if (extra > std::numeric_limits<int>::max() - longer.count_) {
        throw PealError("Слишком много ударов в партии");
    }

    Peal result(longer.count_ + extra);
    int next = 0;
    for (int i = 0; i < pairs; ++i) {
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];
        if (left.bell == right.bell) {
            result.strokes_[next++] = {left.bell, std::max(left.beats, right.beats)};
        } else if (left.bell < right.bell) {
            result.strokes_[next++] = left;
            result.strokes_[next++] = right;
        } else {
            result.strokes_[next++] = right;
            result.strokes_[next++] = left;
        }
    }
    for (int i = pairs; i < longer.count_; ++i) {
        result.strokes_[next++] = longer.strokes_[i];
    }
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) {
        return false;
    }
    for (int i = 0; i < count_; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell || strokes_[i].beats != other.strokes_[i].beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return !(*this == other);
}

Peal::operator int() const {
    std::int64_t total = 0;
    for (int i = 0; i < count_; ++i) {
        total += strokes_[i].beats;
    }
    if (total < std::numeric_limits<int>::min() || total > std::numeric_limits<int>::max()) {
        throw PealError("Сумма долей не помещается в int");
    }
    return static_cast<int>(total);
}

}  // namespace rybinsk
