#include "rybinsk/peal.hpp"

#include <algorithm>
#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Peal count cannot be negative");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("Peal count cannot be negative");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("Peal strokes pointer cannot be null when count > 0");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        count_ = count;
        std::copy_n(strokes, count, strokes_);
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

Peal::Peal(Peal&& other) noexcept
    : strokes_(other.strokes_), count_(other.count_) {
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
        throw PealError("Peal index out of range");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Peal index out of range");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int common = std::min(count_, other.count_);
    int total = std::max(count_, other.count_) - common;
    for (int i = 0; i < common; ++i) {
        total += (strokes_[i].bell == other.strokes_[i].bell) ? 1 : 2;
    }

    Peal result(total);
    int pos = 0;
    for (int i = 0; i < common; ++i) {
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

    if (count_ > common) {
        std::copy(strokes_ + common, strokes_ + count_, result.strokes_ + pos);
    } else if (other.count_ > common) {
        std::copy(other.strokes_ + common, other.strokes_ + other.count_, result.strokes_ + pos);
    }

    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    Peal combined = *this + other;
    *this = std::move(combined);
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