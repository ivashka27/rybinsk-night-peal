
#include "rybinsk/peal.hpp"
#include <utility>

namespace rybinsk {

Peal::Peal() : strokes_(nullptr), count_(0) {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Negative count");
    }

    count_ = count;
    strokes_ = count == 0 ? nullptr : new Stroke[count]{};
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("Invalid strokes");
    }

    count_ = count;
    strokes_ = count == 0 ? nullptr : new Stroke[count];

    for (int i = 0; i < count_; ++i) {
        strokes_[i] = strokes[i];
    }
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;
    strokes_ = count_ == 0 ? nullptr : new Stroke[count_];

    for (int i = 0; i < count_; ++i) {
        strokes_[i] = other.strokes_[i];
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

    Peal temp(other);

    delete[] strokes_;

    strokes_ = temp.strokes_;
    count_ = temp.count_;

    temp.strokes_ = nullptr;
    temp.count_ = 0;

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
    int common = count_ < other.count_ ? count_ : other.count_;

    int resultCount = count_ > other.count_ ? count_ : other.count_;

    for (int i = 0; i < common; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++resultCount;
        }
    }

    Peal result(resultCount);

    int position = 0;

    for (int i = 0; i < common; ++i) {
        Stroke a = strokes_[i];
        Stroke b = other.strokes_[i];

        if (a.bell == b.bell) {
            result.strokes_[position] =
                a.beats >= b.beats ? a : b;
            ++position;
        } else {
            if (a.bell < b.bell) {
                result.strokes_[position++] = a;
                result.strokes_[position++] = b;
            } else {
                result.strokes_[position++] = b;
                result.strokes_[position++] = a;
            }
        }
    }

    for (int i = common; i < count_; ++i) {
        result.strokes_[position++] = strokes_[i];
    }

    for (int i = common; i < other.count_; ++i) {
        result.strokes_[position++] = other.strokes_[i];
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
    int sum = 0;

    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }

    return sum;
}

} // namespace rybinsk
