#include "rybinsk/peal.hpp"
#include "algorithm"

#include <stdexcept>

namespace rybinsk {

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("count < 0");
    }

    if (count == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }

    strokes_ = new Stroke[count];
    count_ = count;

    for (int i = 0; i < count_; i++) {
        strokes_[i].bell = 0;
        strokes_[i].beats = 0;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("count < 0");
    }

    if (count > 0 && strokes == nullptr) {
        throw PealError("strokes == nullptr");
    }

    if (count == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }

    strokes_ = new Stroke[count];
    count_ = count;

    for (int i = 0; i < count_; i++) {
        strokes_[i] = strokes[i];
    }
}

Peal::Peal(const Peal& other) {
    strokes_ = nullptr;
    count_ = 0;

    if (other.count_ == 0) {
        return;
    }

    strokes_ = new Stroke[other.count_];
    count_ = other.count_;

    for (int i = 0; i < count_; i++) {
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
    strokes_ = nullptr;
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other) {
        return *this;
    }

    Stroke* newStrokesPtr = nullptr;
    int newCount = other.count_;

    if (newCount > 0) {
        newStrokesPtr = new Stroke[newCount];
        for (int i = 0; i < newCount; i++) {
            newStrokesPtr[i] = other.strokes_[i];
        }
    }

    delete[] strokes_;

    strokes_ = newStrokesPtr;
    count_ = newCount;

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

    int common = std::min(count_, other.count_);

    int resultCount = 0;
    for (int i = 0; i < common; i++) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            resultCount += 1;
        } else {
            resultCount += 2;
        }
    }
    resultCount += (count_ - common);
    resultCount += (other.count_ - common);

    if (resultCount == 0) {
        return Peal();
    }

    Stroke* resultPtr = new Stroke[resultCount];
    int pos = 0;

    for (int i = 0; i < common; i++) {
        Stroke a = strokes_[i];
        Stroke b = other.strokes_[i];

        if (a.bell == b.bell) {
            if (a.beats >= b.beats) {
                resultPtr[pos] = a;
            } else {
                resultPtr[pos] = b;
            }
            pos += 1;
        } else if (a.bell < b.bell) {
            resultPtr[pos] = a;
            pos += 1;
            resultPtr[pos] = b;
            pos += 1;
        } else {
            resultPtr[pos] = b;
            pos += 1;
            resultPtr[pos] = a;
            pos += 1;
        }
    }

    for (int i = common; i < count_; i++) {
        resultPtr[pos] = strokes_[i];
        pos += 1;
    }

    for (int i = common; i < other.count_; i++) {
        resultPtr[pos] = other.strokes_[i];
        pos += 1;
    }

    Peal result;
    result.strokes_ = resultPtr;
    result.count_ = resultCount;
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    Peal sum = *this + other;

    delete[] strokes_;
    strokes_ = sum.strokes_;
    count_ = sum.count_;

    sum.strokes_ = nullptr;
    sum.count_ = 0;

    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) {
        return false;
    }

    for (int i = 0; i < count_; i++) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            return false;
        }
        if (strokes_[i].beats != other.strokes_[i].beats) {
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