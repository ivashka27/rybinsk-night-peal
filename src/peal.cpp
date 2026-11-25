#include "rybinsk/peal.hpp"

#include <utility>
#include <vector>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    count_ = count;
    if (count > 0) {
        strokes_ = new Stroke[count];
        for (int i = 0; i < count; ++i) {
            strokes_[i] = Stroke{0, 0};
        }
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("null strokes");
    }
    count_ = count;
    if (count > 0) {
        strokes_ = new Stroke[count];
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) : count_(other.count_) {
    if (count_ > 0) {
        strokes_ = new Stroke[count_];
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
    if (this == &other) {
        return *this;
    }
    Stroke* new_strokes = nullptr;
    if (other.count_ > 0) {
        new_strokes = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
            new_strokes[i] = other.strokes_[i];
        }
    }
    delete[] strokes_;
    strokes_ = new_strokes;
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
    std::vector<Stroke> merged;
    const int shared = count_ < other.count_ ? count_ : other.count_;
    for (int i = 0; i < shared; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell == b.bell) {
            if (a.beats >= b.beats) {
                merged.push_back(a);
            } else {
                merged.push_back(b);
            }
        } else if (a.bell < b.bell) {
            merged.push_back(a);
            merged.push_back(b);
        } else {
            merged.push_back(b);
            merged.push_back(a);
        }
    }
    if (count_ > shared) {
        for (int i = shared; i < count_; ++i) {
            merged.push_back(strokes_[i]);
        }
    } else {
        for (int i = shared; i < other.count_; ++i) {
            merged.push_back(other.strokes_[i]);
        }
    }
    return Peal(merged.data(), static_cast<int>(merged.size()));
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
    for (int i = 0; i < count_; ++i) {
        total += strokes_[i].beats;
    }
    return total;
}

}