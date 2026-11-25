#include "rybinsk/peal.hpp"
#include "algorithm"

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Negative peal length");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("Negative peal length");
    } else if (count > 0 && strokes == nullptr) {
        throw PealError("Null pointer with positive length");
    }else if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
        for (int i = 0; i < count; i++) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept {
    strokes_ = other.strokes_;
    count_ = other.count_;

    other.strokes_ = nullptr;
    other.count_ = 0;
}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other) {
        return *this;
    }

    Stroke* newStrokes = nullptr;
    if (other.count_ > 0) {
        newStrokes = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
            newStrokes[i] = other.strokes_[i];
        }
    }

    delete[] strokes_;
    strokes_ = newStrokes;
    count_ = other.count_;
    return *this;
}

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
Peal& Peal::operator=(Peal&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    delete[] strokes_;
    strokes_ = other.strokes_;
    count_ = other.count_;

    other.strokes_ = nullptr;
    other.count_ = 0;
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int index) {
    if (index < 0 || index >= count_) {
        throw PealError("Index out of bounds");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Index out of bounds");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int common = std::min(count_, other.count_);
    const int tail = std::max(count_, other.count_) - common;

    int total = tail;
    for (int i = 0; i < common; i++) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            total += 1;
        } else {
            total += 2;
        }
    }

    Peal result(total);
    int pos = 0;

    for (int i = 0; i < common; i++) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];

        if (a.bell == b.bell) {
            result.strokes_[pos++] = (a.beats >= b.beats) ? a : b;
        } else if (a.bell < b.bell) {
            result.strokes_[pos++] = a;
            result.strokes_[pos++] = b;
        } else {
            result.strokes_[pos++] = b;
            result.strokes_[pos++] = a;
        }
    }

    const Peal& longer = (count_ >= other.count_) ? *this : other;
    for (int i = common; i < longer.count_; ++i) {
        result.strokes_[pos++] = longer.strokes_[i];
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
    if (count_ == 0) {
        return 0;
    } else {
        int sum = 0;
        for (int i = 0; i < count_; i++) {
            sum += strokes_[i].beats;
        }
        return sum;
    }
    
}

}  // namespace rybinsk
