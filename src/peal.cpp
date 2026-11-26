#include "rybinsk/peal.hpp"

#include <algorithm>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("constructor Peal");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) : Peal(count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("constructor Peal");
    }
    for (int i = 0; i < count; i++) {
        strokes_[i] = strokes[i];
    }    
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept 
    : strokes_(other.strokes_), count_(other.count_) {
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

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
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
    if (0 <= index && index < count_)
        return strokes_[index];
    throw PealError("bad idx");
}

const Stroke& Peal::operator[](int index) const {
    if (0 <= index && index < count_)
        return strokes_[index];
    throw PealError("bad idx");
}

Peal Peal::operator+(const Peal& other) const {
    if (count_ >= other.count_) {
        Peal tmp = Peal(count_ + other.count_);
        int self_ptr = 0;
        int other_ptr = 0;
        int tmp_ptr = 0;

        while (other_ptr < other.size()) {
            auto [bell_self, beats_self] = (*this)[self_ptr];
            auto [bell_other, beats_other] = other[other_ptr];

            if (bell_self < bell_other) {
                tmp[tmp_ptr++] = (*this)[self_ptr++];
                tmp[tmp_ptr++] = other[other_ptr++];
            }
            else if (bell_self > bell_other) {
                tmp[tmp_ptr++] = other[other_ptr++];
                tmp[tmp_ptr++] = (*this)[self_ptr++];
            }
            else {
                if (beats_self > beats_other) {
                    tmp[tmp_ptr++] = (*this)[self_ptr];
                }
                else {
                    tmp[tmp_ptr++] = other[other_ptr];
                }
                other_ptr++;
                self_ptr++;
            }
        }

        while (self_ptr < size()) {
            tmp[tmp_ptr++] = (*this)[self_ptr++];
        }

        return Peal(tmp.strokes_, tmp_ptr);
    }
    return other + *this;
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (size() == other.size()) {
        for (int i = 0; i < size(); i++) {
            auto [bell_self, beats_self] = (*this)[i];
            auto [bell_other, beats_other] = other[i];

            if (bell_self != bell_other || beats_self != beats_other) {
                return false;
            }
        }

        return true;
    }
    return false;
}

bool Peal::operator!=(const Peal& other) const {
    return !((*this) == other);
}

Peal::operator int() const {
    if (count_ == 0) {
        return 0;
    }
    int sum = 0;
    for (int i = 0; i < size(); i++) {
        auto [bell, beats] = (*this)[i];
        sum += beats;
    }
    return sum;
}

}  // namespace rybinsk
