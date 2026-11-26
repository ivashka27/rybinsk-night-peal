#include "rybinsk/peal.hpp"

#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative stroke count");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (count > 0 && strokes == nullptr)) {
        throw PealError("invalid stroke array");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        count_ = count;
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
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
    if (this != &other) {
        Peal copy(other);
        std::swap(strokes_, copy.strokes_);
        std::swap(count_, copy.count_);
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
    if (index < 0 || index >= count_) {
        throw PealError("stroke index out of range");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("stroke index out of range");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int paired = count_ < other.count_ ? count_ : other.count_;
    int result_count = count_ > other.count_ ? count_ : other.count_;
    for (int i = 0; i < paired; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++result_count;
        }
    }

    Peal result(result_count);
    int next = 0;
    for (int i = 0; i < paired; ++i) {
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];
        if (left.bell == right.bell) {
            result.strokes_[next++] = left.beats >= right.beats ? left : right;
        } else if (left.bell < right.bell) {
            result.strokes_[next++] = left;
            result.strokes_[next++] = right;
        } else {
            result.strokes_[next++] = right;
            result.strokes_[next++] = left;
        }
    }
    const Peal& longer = count_ > other.count_ ? *this : other;
    for (int i = paired; i < longer.count_; ++i) {
        result.strokes_[next++] = longer.strokes_[i];
    }
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    Peal result = *this + other;
    *this = std::move(result);
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
    int beats = 0;
    for (int i = 0; i < count_; ++i) {
        beats += strokes_[i].beats;
    }
    return beats;
}

}  // namespace rybinsk
