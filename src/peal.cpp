#include "rybinsk/peal.hpp"

#include <utility>

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
        count_ = count;

        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
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
    if (this != &other) {
        Peal temp(other);
        *this = std::move(temp);
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
    int m = count_ < other.count_ ? count_ : other.count_;
    int n = count_ > other.count_ ? count_ : other.count_;
    int extra = 0;

    for (int i = 0; i < m; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++extra;
        }
    }

    Peal result(n + extra);
    int k = 0;

    for (int i = 0; i < m; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];

        if (a.bell == b.bell) {
            result[k++] = a.beats >= b.beats ? a : b;
        } else if (a.bell < b.bell) {
            result[k++] = a;
            result[k++] = b;
        } else {
            result[k++] = b;
            result[k++] = a;
        }
    }

    const Peal& longer = count_ > other.count_ ? *this : other;

    for (int i = m; i < n; ++i) {
        result[k++] = longer[i];
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

}