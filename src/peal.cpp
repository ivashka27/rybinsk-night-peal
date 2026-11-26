#include "rybinsk/peal.hpp"

#include <algorithm>

namespace rybinsk {

Peal::Peal() : strokes_(nullptr), count_(0) {}

Peal::Peal(int count) : strokes_(nullptr), count_(0) {
    if (count < 0) {
        throw PealError("error");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) : Peal(count) {
    if (count > 0 && strokes == nullptr) {
        throw PealError("error");
    }
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = strokes[i];
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
    if (this == &other) {
        return *this;
    }
    Peal a(other);
    std::swap(strokes_, a.strokes_);
    std::swap(count_, a.count_);
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
        throw PealError("error");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("error");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    int a = std::min(count_, other.count_);
    int b = std::max(count_, other.count_);

    int c = 0;
    for (int i = 0; i < a; ++i) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            c += 1;
        } else {
            c += 2;
        }
    }
    c += (b - a);

    Peal d(c);
    int k = 0;

    for (int i = 0; i < a; ++i) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            if (strokes_[i].beats >= other.strokes_[i].beats) {
                d.strokes_[k++] = strokes_[i];
            } else {
                d.strokes_[k++] = other.strokes_[i];
            }
        } else if (strokes_[i].bell < other.strokes_[i].bell) {
            d.strokes_[k++] = strokes_[i];
            d.strokes_[k++] = other.strokes_[i];
        } else {
            d.strokes_[k++] = other.strokes_[i];
            d.strokes_[k++] = strokes_[i];
        }
    }

    for (int i = a; i < b; ++i) {
        if (count_ > other.count_) {
            d.strokes_[k++] = strokes_[i];
        } else {
            d.strokes_[k++] = other.strokes_[i];
        }
    }

    return d;
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
    int a = 0;
    for (int i = 0; i < count_; ++i) {
        a += strokes_[i].beats;
    }
    return a;
}

}  // namespace rybinsk