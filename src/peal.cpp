#include "rybinsk/peal.hpp"

#include <algorithm>
#include <limits>
#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative size");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (count > 0 && strokes == nullptr)) {
        throw PealError("invalid array");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        std::copy_n(strokes, count, strokes_);
        count_ = count;
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

Peal::Peal(Peal&& other) noexcept
    : strokes_(std::exchange(other.strokes_, nullptr)),
      count_(std::exchange(other.count_, 0)) {}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this != &other) {
        Peal copy(other);
        *this = std::move(copy);
    }
    return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
    if (this != &other) {
        delete[] strokes_;
        strokes_ = std::exchange(other.strokes_, nullptr);
        count_ = std::exchange(other.count_, 0);
    }
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
    int pairs = std::min(count_, other.count_);
    long long size = std::max(count_, other.count_);
    for (int i = 0; i < pairs; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++size;
        }
    }
    if (size > std::numeric_limits<int>::max()) {
        throw PealError("too many strokes");
    }

    Peal result(static_cast<int>(size));
    int k = 0;
    for (int i = 0; i < pairs; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell == b.bell) {
            result.strokes_[k++] = a.beats >= b.beats ? a : b;
        } else if (a.bell < b.bell) {
            result.strokes_[k++] = a;
            result.strokes_[k++] = b;
        } else {
            result.strokes_[k++] = b;
            result.strokes_[k++] = a;
        }
    }
    const Peal& longer = count_ >= other.count_ ? *this : other;
    for (int i = pairs; i < longer.count_; ++i) {
        result.strokes_[k++] = longer.strokes_[i];
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
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell != b.bell || a.beats != b.beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return !(*this == other);
}

Peal::operator int() const {
    long long sum = 0;
    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }
    return static_cast<int>(sum);
}

}  // namespace rybinsk
