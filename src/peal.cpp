#include "rybinsk/peal.hpp"

#include <utility>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]();
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("null strokes");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        count_ = count;
        for (int i = 0; i < count_; i++) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

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
    Peal tmp(other);
    *this = std::move(tmp);
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
    int common = count_;
    if (other.count_ < common) {
        common = other.count_;
    }
    int total = (count_ - common) + (other.count_ - common);
    for (int i = 0; i < common; i++) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            total += 1;
        } else {
            total += 2;
        }
    }
    Peal res(total);
    int pos = 0;
    for (int i = 0; i < common; i++) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell == b.bell) {
            if (a.beats >= b.beats) {
                res.strokes_[pos] = a;
            } else {
                res.strokes_[pos] = b;
            }
            pos++;
        } else if (a.bell < b.bell) {
            res.strokes_[pos] = a;
            pos++;
            res.strokes_[pos] = b;
            pos++;
        } else {
            res.strokes_[pos] = b;
            pos++;
            res.strokes_[pos] = a;
            pos++;
        }
    }
    for (int i = common; i < count_; i++) {
        res.strokes_[pos] = strokes_[i];
        pos++;
    }
    for (int i = common; i < other.count_; i++) {
        res.strokes_[pos] = other.strokes_[i];
        pos++;
    }
    return res;
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
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

}  // namespace rybinsk