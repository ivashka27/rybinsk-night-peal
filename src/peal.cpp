#include "rybinsk/peal.hpp"
#include <algorithm>
#include <utility>
#include <vector>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("отрицательная длина");
    }
    if (count == 0) {
        return;
    }
    strokes_ = new Stroke[count]{};
    count_ = count;
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("отрицательная длина или пустой массив ударов");
    }
    if (count == 0) {
        return;
    }
    strokes_ = new Stroke[count];
    count_ = count;
    std::copy(strokes, strokes + count, strokes_);
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
    Peal copy(other);
    std::swap(strokes_, copy.strokes_);
    std::swap(count_, copy.count_);
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
        throw PealError("неверный индекс");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("неверный индекс");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int common = std::min(count_, other.count_);
    std::vector<Stroke> merged;
    merged.reserve(static_cast<std::size_t>(count_) + static_cast<std::size_t>(other.count_));

    for (int i = 0; i < common; i++) {
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];
        if (left.bell == right.bell) {
            if (left.beats >= right.beats) {
                merged.push_back(left);
            } else {
                merged.push_back(right);
            }
        } else if (left.bell < right.bell) {
            merged.push_back(left);
            merged.push_back(right);
        } else {
            merged.push_back(right);
            merged.push_back(left);
        }
    }

    for (int i = common; i < count_; i++) {
        merged.push_back(strokes_[i]);
    }
    for (int i = common; i < other.count_; i++) {
        merged.push_back(other.strokes_[i]);
    }
    return Peal(merged.data(), static_cast<int>(merged.size()));
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
    int total = 0;
    for (int i = 0; i < count_; i++) {
        total += strokes_[i].beats;
    }
    return total;
}
}  // namespace rybinsk
