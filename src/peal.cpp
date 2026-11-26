
#include "rybinsk/peal.hpp"

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative count");
    }

    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("invalid strokes");
    }

    if (count > 0) {
        strokes_ = new Stroke[count];
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
        count_ = count;
    }
}

Peal::Peal(const Peal& other)
    : Peal(other.strokes_, other.count_) {}

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
    Stroke* old = strokes_;
    strokes_ = copy.strokes_;
    count_ = copy.count_;
    copy.strokes_ = old;
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
    if (this == &other) {
        return Peal(*this);
    }

    const int common = (count_ < other.count_) ? count_ : other.count_;
    const int total = (count_ > other.count_) ? count_ : other.count_;

    Peal result(total + common);

    int pos = 0;

    for (int i = 0; i < common; ++i) {
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];

        if (left.bell == right.bell) {
            result.strokes_[pos++] =
                (left.beats >= right.beats) ? left : right;
        } else if (left.bell < right.bell) {
            result.strokes_[pos++] = left;
            result.strokes_[pos++] = right;
        } else {
            result.strokes_[pos++] = right;
            result.strokes_[pos++] = left;
        }
    }

    if (count_ > common) {
        for (int i = common; i < count_; ++i) {
            result.strokes_[pos++] = strokes_[i];
        }
    } else {
        for (int i = common; i < other.count_; ++i) {
            result.strokes_[pos++] = other.strokes_[i];
        }
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
    int total = 0;
    for (int i = 0; i < count_; ++i) {
        total += strokes_[i].beats;
    }
    return total;
}

}  // namespace rybinsk
