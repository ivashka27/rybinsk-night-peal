#include "rybinsk/peal.hpp"
#include <algorithm>
#include <vector>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Отрицательное количество ударов");
    }
    if (count > 0) {
        count_ = count;
        strokes_ = new Stroke[count_](); //заполняет нулями
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("Отрицательное количество");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("Нулевой указатель при ненулевом размере");
    }

    if (count > 0) {
        count_ = count;
        strokes_ = new Stroke[count_];
        std::copy(strokes, strokes + count, strokes_);
    }
}

Peal::Peal(const Peal& other) : count_(other.count_) {
    if (count_ > 0) {
        strokes_ = new Stroke[count_];
        std::copy(other.strokes_, other.strokes_ + count_, strokes_);
    }
}

Peal::Peal(Peal&& other) noexcept : strokes_(other.strokes_), count_(other.count_) {
    other.strokes_ = nullptr;
    other.count_ = 0;
}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this != &other) { // Защита от a = a
        Stroke* new_strokes = nullptr;
        if (other.count_ > 0) {
            new_strokes = new Stroke[other.count_];
            std::copy(other.strokes_, other.strokes_ + other.count_, new_strokes);
        }
        delete[] strokes_; // Удаляем старые только после успешного выделения новых
        strokes_ = new_strokes;
        count_ = other.count_;
    }
    return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
    if (this != &other) { // Защита от a = std::move(a)
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
    if (index < 0 || index >= count_) {
        throw PealError("Индекс за границами");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Индекс за границами");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    std::vector<Stroke> result;

    int max_size = std::max(count_, other.count_);
    for (int i = 0; i < max_size; ++i) {
        if (i < count_ && i < other.count_) {
            Stroke a = strokes_[i];
            Stroke b = other.strokes_[i];

            if (a.bell == b.bell) {
                //остается тот, что длиннее
                result.push_back(a.beats >= b.beats ? a : b);
            } else {
                //сначала меньший, потом больший
                if (a.bell < b.bell) {
                    result.push_back(a);
                    result.push_back(b);
                } else {
                    result.push_back(b);
                    result.push_back(a);
                }
            }
        } else if (i < count_) {
            result.push_back(strokes_[i]);
        } else {
            result.push_back(other.strokes_[i]);
        }
    }
    return Peal(result.data(), static_cast<int>(result.size()));
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) return false;
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
    int sum = 0;
    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }
    return sum;
}

}