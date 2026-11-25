#include "rybinsk/peal.hpp"

#include <algorithm>
#include <vector>

namespace rybinsk {

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("count cannot be negative");
    }
    if (count == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }
    count_ = count;
    strokes_ = new Stroke[count_]();
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("invalid strokes array or count");
    }
    if (count == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }
    count_ = count;
    strokes_ = new Stroke[count_];
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = strokes[i];
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }
    count_ = other.count_;
    strokes_ = new Stroke[count_];
    for (int i = 0; i < count_; ++i) {
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
    count_ = 0;
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other) {
        return *this;
    }
    if (other.count_ == 0) {
        delete[] strokes_;
        strokes_ = nullptr;
        count_ = 0;
        return *this;
    }
    Stroke* new_strokes = new Stroke[other.count_];
    for (int i = 0; i < other.count_; ++i) {
        new_strokes[i] = other.strokes_[i];
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
    if (count_ == 0) {
        return other;
    }
    if (other.count_ == 0) {
        return *this;
    }

    std::vector<Stroke> result;
    int min_len = std::min(count_, other.count_);
    for (int i = 0; i < min_len; ++i) {
        const Stroke& s1 = strokes_[i];
        const Stroke& s2 = other.strokes_[i];
        if (s1.bell == s2.bell) {
            result.push_back(Stroke{s1.bell, std::max(s1.beats, s2.beats)});
        } else if (s1.bell < s2.bell) {
            result.push_back(s1);
            result.push_back(s2);
        } else {
            result.push_back(s2);
            result.push_back(s1);
        }
    }

    if (count_ > min_len) {
        for (int i = min_len; i < count_; ++i) {
            result.push_back(strokes_[i]);
        }
    } else if (other.count_ > min_len) {
        for (int i = min_len; i < other.count_; ++i) {
            result.push_back(other.strokes_[i]);
        }
    }

    return Peal(result.data(), static_cast<int>(result.size()));
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
        if (strokes_[i].bell != other.strokes_[i].bell || strokes_[i].beats != other.strokes_[i].beats) {
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
