#include "rybinsk/peal.hpp"

#include <algorithm>
#include <limits>
#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("отрицательная длина партии");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("отрицательная длина партии");
    }
    if (count > 0) {
        if (strokes == nullptr) {
            throw PealError("нет массива ударов");
        }
        strokes_ = new Stroke[count];
        count_ = count;
        std::copy_n(strokes, count, strokes_);
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

Peal::Peal(Peal&& other) noexcept : strokes_(other.strokes_), count_(other.count_) {
    other.strokes_ = nullptr;
    other.count_ = 0;
}

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
        throw PealError("нет удара с таким индексом");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("нет удара с таким индексом");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int pairs = std::min(count_, other.count_);
    const Peal& longer = count_ >= other.count_ ? *this : other;

    long long result_count = longer.count_;
    for (int i = 0; i < pairs; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++result_count;
        }
    }
    if (result_count > std::numeric_limits<int>::max()) {
        throw PealError("слишком длинная партия");
    }

    Peal result(static_cast<int>(result_count));
    int next = 0;

    for (int i = 0; i < pairs; ++i) {
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];

        if (left.bell == right.bell) {
            result.strokes_[next++] = Stroke{left.bell, std::max(left.beats, right.beats)};
        } else if (left.bell < right.bell) {
            result.strokes_[next++] = left;
            result.strokes_[next++] = right;
        } else {
            result.strokes_[next++] = right;
            result.strokes_[next++] = left;
        }
    }

    for (int i = pairs; i < longer.count_; ++i) {
        result.strokes_[next++] = longer.strokes_[i];
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
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];
        if (left.bell != right.bell || left.beats != right.beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return !(*this == other);
}

Peal::operator int() const {
    long long beats = 0;
    for (int i = 0; i < count_; ++i) {
        beats += strokes_[i].beats;
    }
    return static_cast<int>(beats);
}

}