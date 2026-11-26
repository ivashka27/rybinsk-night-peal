#include "rybinsk/peal.hpp"
#include <utility>
#include <vector>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
    if (count < 0)
        throw PealError("Invalid count");

    if (count > 0)
        strokes_ = new Stroke[count]{};

    count_ = count;
}

Peal::Peal(const Stroke* strokes, int count) : Peal(count) {
    if (count > 0 && !strokes)
        throw PealError("Null array");

    for (int i = 0; i < count_; ++i)
        strokes_[i] = strokes[i];
}

Peal::Peal(const Peal& other)
    : Peal(other.strokes_, other.count_) {}

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
        Peal copy(other);
        *this = std::move(copy);
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
    if (index < 0 || index >= count_)
        throw PealError("Invalid index");

    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_)
        throw PealError("Invalid index");

    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    std::vector<Stroke> result;
    int i = 0;

    for (; i < count_ && i < other.count_; ++i) {
        Stroke a = strokes_[i];
        Stroke b = other.strokes_[i];

        if (a.bell == b.bell) {
            result.push_back(a.beats >= b.beats ? a : b);
        } else if (a.bell < b.bell) {
            result.push_back(a);
            result.push_back(b);
        } else {
            result.push_back(b);
            result.push_back(a);
        }
    }

    for (int j = i; j < count_; ++j)
        result.push_back(strokes_[j]);

    for (int j = i; j < other.count_; ++j)
        result.push_back(other.strokes_[j]);

    return Peal(result.data(),
                static_cast<int>(result.size()));
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_)
        return false;

    for (int i = 0; i < count_; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell ||
            strokes_[i].beats != other.strokes_[i].beats)
            return false;
    }

    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return !(*this == other);
}

Peal::operator int() const {
    int sum = 0;

    for (int i = 0; i < count_; ++i)
        sum += strokes_[i].beats;

    return sum;
}

}