#include "rybinsk/peal.hpp"

namespace rybinsk {

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Negative count");
    }

    count_ = count;
    strokes_ = count > 0 ? new Stroke[count]{} : nullptr;
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("Invalid array");
    }

    count_ = count;
    strokes_ = count > 0 ? new Stroke[count] : nullptr;

    for (int i = 0; i < count_; i++) {
        strokes_[i] = strokes[i];
    }
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;
    strokes_ = count_ > 0 ? new Stroke[count_] : nullptr;

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
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other) {
        return *this;
    }

    Stroke* newStrokes = nullptr;

    if (other.count_ > 0) {
        newStrokes = new Stroke[other.count_];

        for (int i = 0; i < other.count_; i++) {
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
    Peal result(count_ * 2 + other.count_ * 2);

    int resultIndex = 0;

    int commonCount = count_;
    if (other.count_ < commonCount) {
        commonCount = other.count_;
    }

    for (int i = 0; i < commonCount; i++) {
        Stroke first = strokes_[i];
        Stroke second = other.strokes_[i];

        if (first.bell == second.bell) {
            if (first.beats >= second.beats) {
                result.strokes_[resultIndex] = first;
            } else {
                result.strokes_[resultIndex] = second;
            }

            resultIndex++;
        } else {
            if (first.bell < second.bell) {
                result.strokes_[resultIndex] = first;
                result.strokes_[resultIndex + 1] = second;
            } else {
                result.strokes_[resultIndex] = second;
                result.strokes_[resultIndex + 1] = first;
            }

            resultIndex += 2;
        }
    }

    for (int i = commonCount; i < count_; i++) {
        result.strokes_[resultIndex] = strokes_[i];
        resultIndex++;
    }

    for (int i = commonCount; i < other.count_; i++) {
        result.strokes_[resultIndex] = other.strokes_[i];
        resultIndex++;
    }

    result.count_ = resultIndex;

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

    for (int i = 0; i < count_; i++) {
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

    for (int i = 0; i < count_; i++) {
        sum += strokes_[i].beats;
    }

    return sum;
}

}  // namespace rybinsk