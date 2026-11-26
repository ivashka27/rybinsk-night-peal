#include "rybinsk/peal.hpp"

#include <algorithm>
#include <utility>

namespace rybinsk {
namespace {

void checkIndex(int ind, int count) {
    if (ind < 0 || ind >= count) {
        throw PealError("error!");
    }
}
}  // namespace
Peal::Peal() = default;
Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("error!");
    }
    if (count == 0) {
        return;
    }
    strokes_ = new Stroke[count]{};
    count_ = count;
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("error!");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("error!");
    }
    if (count == 0) {
        return;
    }
    strokes_ = new Stroke[count];
    count_ = count;
    for (int i = 0; i < count; ++i) {
        strokes_[i] = strokes[i];
    }
}
Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}
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
    Peal copy(other);
    std::swap(strokes_, copy.strokes_);
    std::swap(count_, copy.count_);
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
    checkIndex(index, count_);
    return strokes_[index];
}
const Stroke& Peal::operator[](int index) const {
    checkIndex(index, count_);
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int shared = std::min(count_, other.count_);
    int result_count = std::max(count_, other.count_);
    for (int i = 0; i < shared; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++result_count;
        }
    }
    Peal result(result_count);
    int out = 0;
    for (int i = 0; i < shared; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell < b.bell) {
            result.strokes_[out++] = a;
            result.strokes_[out++] = b;
        } else if (b.bell < a.bell) {
            result.strokes_[out++] = b;
            result.strokes_[out++] = a;
        } else {
            result.strokes_[out++] = Stroke{a.bell, std::max(a.beats, b.beats)};
        }
    }
    const Stroke* tail = count_ > other.count_ ? strokes_ : other.strokes_;
    const int total = std::max(count_, other.count_);
    for (int i = shared; i < total; ++i) {
        result.strokes_[out++] = tail[i];
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
