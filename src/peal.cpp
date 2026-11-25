#include "rybinsk/peal.hpp"
#include <utility>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Wrong count");
    }

    count_ = count;

    if (count > 0) {
        strokes_ = new Stroke[count]{};
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("Wrong array");
    }

    count_ = count;

    if (count > 0) {
        strokes_ = new Stroke[count];

        for (int i = 0; i < count; i++) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other)
    : Peal(other.strokes_, other.count_) {}

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

    Peal a(other);
    *this = std::move(a);

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
        throw PealError("Wrong index");
    }

    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Wrong index");
    }

    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    int n = count_;

    if (other.count_ < n) {
        n = other.count_;
    }

    int cnt = count_ + other.count_;

    for (int i = 0; i < n; i++) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            cnt--;
        }
    }

    Peal a(cnt);
    int j = 0;

    for (int i = 0; i < n; i++) {
        Stroke x = strokes_[i];
        Stroke y = other.strokes_[i];

        if (x.bell == y.bell) {
            if (x.beats >= y.beats) {
                a[j++] = x;
            } else {
                a[j++] = y;
            }
        } else if (x.bell < y.bell) {
            a[j++] = x;
            a[j++] = y;
        } else {
            a[j++] = y;
            a[j++] = x;
        }
    }

    for (int i = n; i < count_; i++) {
        a[j++] = strokes_[i];
    }

    for (int i = n; i < other.count_; i++) {
        a[j++] = other.strokes_[i];
    }

    return a;
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
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

}