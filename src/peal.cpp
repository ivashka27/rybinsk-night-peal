#include "rybinsk/peal.hpp"

#include <algorithm>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Negative peal size");
    }
    if (count == 0) {
        return;
    }
    strokes_ = new Stroke[count]{};
    count_ = count;
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("Invalid strokes array or count");
    }
    if (count == 0) {
        return;
    }
    strokes_ = new Stroke[count];
    count_ = count;
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = strokes[i];
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ == 0) {
        return;
    }
    strokes_ = new Stroke[other.count_];
    count_ = other.count_;
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = other.strokes_[i];
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

    Stroke* new_strokes = nullptr;
    if (other.count_ > 0) {
        new_strokes = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
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
    const int min_len = std::min(count_, other.count_);
    const int max_len = std::max(count_, other.count_);

    int total_strokes = max_len - min_len;
    for (int i = 0; i < min_len; ++i) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            total_strokes += 1;
        } else {
            total_strokes += 2;
        }
    }

    Peal result(total_strokes);
    int pos = 0;

    for (int i = 0; i < min_len; ++i) {
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

    const Peal& longer = (count_ >= other.count_) ? *this : other;
    for (int i = min_len; i < max_len; ++i) {
        result.strokes_[pos++] = longer.strokes_[i];
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
