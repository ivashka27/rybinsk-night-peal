#include "rybinsk/peal.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace rybinsk {
namespace {

[[noreturn]] void notReady() {
    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
    throw std::logic_error("ещё не написано");
}

}  // namespace

Peal::Peal() {}

Peal::Peal(int n) {
    if (n < 0) throw PealError("n < 0");
    if (n > 0) {
        strokes_ = new Stroke[n]();
        count_ = n;
    }
}

Peal::Peal(const Stroke* p, int n) {
    if (n < 0 || (n > 0 && p == nullptr)) throw PealError("bad args");
    if (n > 0) {
        strokes_ = new Stroke[n];
        count_ = n;
        std::copy(p, p + n, strokes_);
    }
}

Peal::Peal(const Peal& o) {
    if (o.count_ > 0) {
        strokes_ = new Stroke[o.count_];
        count_ = o.count_;
        std::copy(o.strokes_, o.strokes_ + o.count_, strokes_);
    }
}

Peal::Peal(Peal&& o) noexcept {
    strokes_ = o.strokes_;
    count_ = o.count_;
    o.strokes_ = nullptr;
    o.count_ = 0;
}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& o) {
    if (this != &o) {
        Stroke* t = o.count_ > 0 ? new Stroke[o.count_] : nullptr;
        std::copy(o.strokes_, o.strokes_ + o.count_, t);
        delete[] strokes_;
        strokes_ = t;
        count_ = o.count_;
    }
    return *this;
}

Peal& Peal::operator=(Peal&& o) noexcept {
    if (this != &o) {
        delete[] strokes_;
        strokes_ = o.strokes_;
        count_ = o.count_;
        o.strokes_ = nullptr;
        o.count_ = 0;
    }
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int i) {
    if (i < 0 || i >= count_) throw PealError("index");
    return strokes_[i];
}

const Stroke& Peal::operator[](int i) const {
    if (i < 0 || i >= count_) throw PealError("index");
    return strokes_[i];
}

Peal Peal::operator+(const Peal& o) const {
    std::vector<Stroke> v;
    int m = std::min(count_, o.count_);
    for (int i = 0; i < m; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = o.strokes_[i];
        if (a.bell != b.bell) {
            if (a.bell < b.bell) {
                v.push_back(a);
                v.push_back(b);
            } else {
                v.push_back(b);
                v.push_back(a);
            }
        } else {
            v.push_back(a.beats >= b.beats ? a : b);
        }
    }
    for (int i = m; i < count_; ++i) v.push_back(strokes_[i]);
    for (int i = m; i < o.count_; ++i) v.push_back(o.strokes_[i]);
    return Peal(v.data(), static_cast<int>(v.size()));
}

Peal& Peal::operator+=(const Peal& o) {
    *this = *this + o;
    return *this;
}

bool Peal::operator==(const Peal& o) const {
    if (count_ != o.count_) return false;
    for (int i = 0; i < count_; ++i) {
        if (strokes_[i].bell != o.strokes_[i].bell ||
            strokes_[i].beats != o.strokes_[i].beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& o) const {
    return !(*this == o);
}

Peal::operator int() const {
    int s = 0;
    for (int i = 0; i < count_; ++i) s += strokes_[i].beats;
    return s;
}

}  // namespace rybinsk