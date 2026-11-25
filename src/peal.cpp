
#include "rybinsk/peal.hpp"

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Negative count");
    }

    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("Invalid strokes");
    }

    if (count > 0) {
        strokes_ = new Stroke[count];
        count_ = count;

        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ > 0) {
        strokes_ = new Stroke[other.count_];
        count_ = other.count_;

        for (int i = 0; i < count_; ++i) {
            strokes_[i] = other.strokes_[i];
        }
    }
}

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
        throw PealError("Index out of range");
    }

    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Index out of range");
    }

    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int common = count_ < other.count_ ? count_ : other.count_;
    const int maximum = count_ > other.count_ ? count_ : other.count_;

    Peal result(count_ + other.count_);
    int pos = 0;

    for (int i = 0; i < common; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];

        if (a.bell == b.bell) {
            result.strokes_[pos++] =
                (a.beats >= b.beats) ? a : b;
        } else if (a.bell < b.bell) {
            result.strokes_[pos++] = a;
            result.strokes_[pos++] = b;
        } else {
            result.strokes_[pos++] = b;
            result.strokes_[pos++] = a;
        }
    }

    for (int i = common; i < maximum; ++i) {
        result.strokes_[pos++] =
            (count_ > other.count_) ? strokes_[i] : other.strokes_[i];
    }

    result.count_ = pos;
    return result;
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
