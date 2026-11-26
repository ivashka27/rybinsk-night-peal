#include "rybinsk/peal.hpp"

namespace rybinsk {
Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("wrong count");
    }
    count_ = count;
    if (count_ == 0) {
        strokes_ = nullptr;
    } else {
        strokes_ = new Stroke[count_]{};
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("wrong data");
    }
    count_ = count;
    if (count_ == 0) {
        strokes_ = nullptr;
    } else {
        strokes_ = new Stroke[count_];
        for (int i = 0; i < count_; i++) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;
    if (count_ == 0) {
        strokes_ = nullptr;
    } else {
        strokes_ = new Stroke[count_];
        for (int i = 0; i < count_; i++) {
            strokes_[i] = other.strokes_[i];
        }
    }
}

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
    Stroke* new_strokes = nullptr;
    if (other.count_ > 0) {
        new_strokes = new Stroke[other.count_];
        for (int i = 0; i < other.count_; i++) {
            new_strokes[i] = other.strokes_[i];
        }
    }
    delete[] strokes_;
    strokes_ = new_strokes;
    count_ = other.count_;
    return *this;
}

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

Stroke& Peal::operator[](int id) {
    if (id < 0 || id >= count_) {
        throw PealError("wrong id");
    }
    return strokes_[id];
}

const Stroke& Peal::operator[](int id) const {
    if (id < 0 || id >= count_) {
        throw PealError("wrong id");
    }
    return strokes_[id];
}

Peal Peal::operator+(const Peal& other) const {
    int common = count_;
    if (other.count_ < common) {
        common = other.count_;
    }
    int result_count = count_ + other.count_ - common;
    if (result_count == 0) {
        return Peal();
    }
    Stroke* result = new Stroke[result_count];
    int position = 0;
    for (int i = 0; i < common; i++) {
        Stroke first = strokes_[i];
        Stroke second = other.strokes_[i];
        if (first.bell == second.bell) {
            if (first.beats >= second.beats) {
                result[position] = first;
            } else {
                result[position] = second;
            }
            position++;
        } else {
            if (first.bell < second.bell) {
                result[position] = first;
                result[position + 1] = second;
            } else {
                result[position] = second;
                result[position + 1] = first;
            }
            position += 2;
        }
    } 
    for (int i = common; i < count_; i++) {
        result[position] = strokes_[i];
        position++;
    }
    for (int i = common; i < other.count_; i++) {
        result[position] = other.strokes_[i];
        position++;
    }
    Peal answer(result, position);
    delete[] result;
    return answer;
}

Peal& Peal::operator+=(const Peal& other) {
    Peal result = *this + other;
    *this = static_cast<Peal&&>(result);
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) {
        return false;
    }
    for (int i = 0; i < count_; i++) {
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
    for (int i = 0; i < count_; i++) {
        sum += strokes_[i].beats;
    }
    return sum;
}

}