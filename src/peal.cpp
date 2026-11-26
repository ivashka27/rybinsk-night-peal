#include "rybinsk/peal.hpp"

#include <algorithm>
#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative stroke count");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("negative stroke count");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("null pointer with positive count");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        count_ = count;
        for (int i = 0; i < count_; ++i) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ > 0) {
        strokes_ = new Stroke[other.count_];
        count_ = other.count_;
        for (int i = 0; i < count_; ++i) {
            strokes_[i] = other.strokes_[i];
        }
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
    if (this != &other) {
        Peal temp(other);
        std::swap(strokes_, temp.strokes_);
        std::swap(count_, temp.count_);
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
    const int min_len = std::min(count_, other.count_);
    int total_len = 0;
    for (int i = 0; i < min_len; ++i) {
        total_len += (strokes_[i].bell != other.strokes_[i].bell) ? 2 : 1;
    }
    total_len += (count_ - min_len) + (other.count_ - min_len);

    Peal result(total_len);
    int pos = 0;

    for (int i = 0; i < min_len; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell != b.bell) {
            if (a.bell < b.bell) {
                result.strokes_[pos++] = a;
                result.strokes_[pos++] = b;
            } else {
                result.strokes_[pos++] = b;
                result.strokes_[pos++] = a;
            }
        } else {
            result.strokes_[pos++] = Stroke{a.bell, std::max(a.beats, b.beats)};
        }
    }

    for (int i = min_len; i < count_; ++i) {
        result.strokes_[pos++] = strokes_[i];
    }
    for (int i = min_len; i < other.count_; ++i) {
        result.strokes_[pos++] = other.strokes_[i];
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
    int total_beats = 0;
    for (int i = 0; i < count_; ++i) {
        total_beats += strokes_[i].beats;
    }
    return total_beats;
}

}  // namespace rybinsk