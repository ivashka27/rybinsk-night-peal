#include "rybinsk/peal.hpp"
#include <algorithm>
#include <vector>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) : count_(count) {
    if (count < 0) throw PealError("Negative count");
    if (count > 0) strokes_ = new Stroke[count_]{};
}

Peal::Peal(const Stroke* strokes, int count) : count_(count) {
    if (count < 0 || (strokes == nullptr && count > 0)) throw PealError("Bad input");
    if (count > 0) {
        strokes_ = new Stroke[count_];
        std::copy(strokes, strokes + count, strokes_);
    }
}

Peal::Peal(const Peal& other) : count_(other.count_) {
    if (count_ > 0) {
        strokes_ = new Stroke[count_];
        std::copy(other.strokes_, other.strokes_ + count_, strokes_);
    }
}

Peal::Peal(Peal&& other) noexcept : strokes_(other.strokes_), count_(other.count_) {
    other.strokes_ = nullptr;
    other.count_ = 0;
}

Peal::~Peal() { delete[] strokes_; }

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

int Peal::size() const { return count_; }

Stroke& Peal::operator[](int index) {
    if (index < 0 || index >= count_) throw PealError("Index out of range");
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) throw PealError("Index out of range");
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    if (count_ == 0) return other;
    if (other.count_ == 0) return *this;

    std::vector<Stroke> res;
    res.reserve(count_ + other.count_);
    int min_len = std::min(count_, other.count_);

    for (int i = 0; i < min_len; ++i) {
        const Stroke& s1 = strokes_[i];
        const Stroke& s2 = other.strokes_[i];
        if (s1.bell == s2.bell) {
            res.push_back(s1.beats >= s2.beats ? s1 : s2);
        } else {
            if (s1.bell < s2.bell) {
                res.push_back(s1);
                res.push_back(s2);
            } else {
                res.push_back(s2);
                res.push_back(s1);
            }
        }
    }
    for (int i = min_len; i < count_; ++i) res.push_back(strokes_[i]);
    for (int i = min_len; i < other.count_; ++i) res.push_back(other.strokes_[i]);

    Peal result(static_cast<int>(res.size()));
    for (size_t i = 0; i < res.size(); ++i) result.strokes_[i] = res[i];
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) return false;
    for (int i = 0; i < count_; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell || strokes_[i].beats != other.strokes_[i].beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const { return !(*this == other); }

Peal::operator int() const {
    int sum = 0;
    for (int i = 0; i < count_; ++i) sum += strokes_[i].beats;
    return sum;
}

}  // namespace rybinsk