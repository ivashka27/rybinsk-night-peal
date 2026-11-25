#include "rybinsk/peal.hpp"

#include <utility>

namespace rybinsk {

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }

    strokes_ = new Stroke[count];
    count_ = count;
    for (int i = 0; i < count_; ++i) {
        strokes_[i].bell = 0;
        strokes_[i].beats = 0;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("null strokes pointer");
    }
    if (count == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }

    strokes_ = new Stroke[count];
    count_ = count;
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = strokes[i];
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }

    strokes_ = new Stroke[other.count_];
    count_ = other.count_;
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = other.strokes_[i];
    }
}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept {
    strokes_ = other.strokes_;
    count_ = other.count_;
    other.strokes_ = nullptr;
    other.count_ = 0;
}

Peal::~Peal() {
    // Освободите strokes_. delete[] nullptr вызывать можно.
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other) {
        return *this;
    }

    Stroke* next = nullptr;
    if (other.count_ > 0) {
        next = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
            next[i] = other.strokes_[i];
        }
    }

    delete[] strokes_;
    strokes_ = next;
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
        throw PealError("index out of range");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("index out of range");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    int pairs = count_;
    if (other.count_ < pairs) {
        pairs = other.count_;
    }

    int out_len = (count_ - pairs) + (other.count_ - pairs);
    for (int i = 0; i < pairs; ++i) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            out_len += 1;
        } else {
            out_len += 2;
        }
    }

    Peal result(out_len);
    int k = 0;
    for (int i = 0; i < pairs; ++i) {
        Stroke a = strokes_[i];
        Stroke b = other.strokes_[i];
        if (a.bell == b.bell) {
            if (a.beats >= b.beats) {
                result.strokes_[k] = a;
            } else {
                result.strokes_[k] = b;
            }
            k += 1;
        } else if (a.bell < b.bell) {
            result.strokes_[k] = a;
            result.strokes_[k + 1] = b;
            k += 2;
        } else {
            result.strokes_[k] = b;
            result.strokes_[k + 1] = a;
            k += 2;
        }
    }

    for (int i = pairs; i < count_; ++i) {
        result.strokes_[k] = strokes_[i];
        k += 1;
    }
    for (int i = pairs; i < other.count_; ++i) {
        result.strokes_[k] = other.strokes_[i];
        k += 1;
    }

    return result;
}

Peal& Peal::operator+=(const Peal& other) {
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
    int total = 0;
    for (int i = 0; i < count_; ++i) {
        total += strokes_[i].beats;
    }
    return total;
}

}  // namespace rybinsk