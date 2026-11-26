#include "rybinsk/peal.hpp"

#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int n) {
    if (n < 0) throw PealError("negative count");

    if (n > 0) {
        strokes_ = new Stroke[n]{};
        count_ = n;
    }
}

Peal::Peal(const Stroke* s, int n) {
    if (n < 0 || (s == nullptr && n > 0)) {
        throw PealError("invalid strokes");
    }

    if (n > 0) {
        strokes_ = new Stroke[n];
        for (int i = 0; i < n; ++i) {
            strokes_[i] = s[i];
        }
        count_ = n;
    }
}

Peal::Peal(const Peal& p) : Peal(p.strokes_, p.count_) {}

Peal::Peal(Peal&& p) noexcept
    : strokes_(p.strokes_), count_(p.count_) {
    p.strokes_ = nullptr;
    p.count_ = 0;
}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& p) {
    if (this != &p) {
        Peal tmp(p);
        std::swap(strokes_, tmp.strokes_);
        std::swap(count_, tmp.count_);
    }
    return *this;
}

Peal& Peal::operator=(Peal&& p) noexcept {
    if (this != &p) {
        delete[] strokes_;
        strokes_ = p.strokes_;
        count_ = p.count_;
        p.strokes_ = nullptr;
        p.count_ = 0;
    }
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int i) {
    if (i < 0 || i >= count_) {
        throw PealError("index out of range");
    }
    return strokes_[i];
}

const Stroke& Peal::operator[](int i) const {
    if (i < 0 || i >= count_) {
        throw PealError("index out of range");
    }
    return strokes_[i];
}

Peal Peal::operator+(const Peal& p) const {
    int n = count_ < p.count_ ? count_ : p.count_;
    int m = count_ > p.count_ ? count_ : p.count_;

    Peal res(m + n);
    int j = 0;

    for (int i = 0; i < n; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = p.strokes_[i];

        if (a.bell == b.bell) {
            res.strokes_[j++] = a.beats >= b.beats ? a : b;
        } else if (a.bell < b.bell) {
            res.strokes_[j++] = a;
            res.strokes_[j++] = b;
        } else {
            res.strokes_[j++] = b;
            res.strokes_[j++] = a;
        }
    }

    for (int i = n; i < count_; ++i) {
        res.strokes_[j++] = strokes_[i];
    }

    for (int i = n; i < p.count_; ++i) {
        res.strokes_[j++] = p.strokes_[i];
    }

    res.count_ = j;
    return res;
}

Peal& Peal::operator+=(const Peal& p) {
    Peal res = *this + p;
    *this = std::move(res);
    return *this;
}

bool Peal::operator==(const Peal& p) const {
    if (count_ != p.count_) return false;

    for (int i = 0; i < count_; ++i) {
        if (strokes_[i].bell != p.strokes_[i].bell ||
            strokes_[i].beats != p.strokes_[i].beats) {
            return false;
        }
    }

    return true;
}

bool Peal::operator!=(const Peal& p) const {
    return !(*this == p);
}

Peal::operator int() const {
    int sum = 0;

    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }

    return sum;
}

}