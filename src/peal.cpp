#include "rybinsk/peal.hpp"

#include <utility>
#include <vector>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("отрицательная длина партии");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) : Peal(count) {
    if (count > 0 && strokes == nullptr) {
        throw PealError("пустой указатель на удары");
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
    if (this != &other) {
        Peal copy(other);
        std::swap(strokes_, copy.strokes_);
        std::swap(count_, copy.count_);
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
        throw PealError("нет удара с таким номером");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("нет удара с таким номером");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    std::vector<Stroke> out;
    out.reserve(static_cast<std::size_t>(count_) + static_cast<std::size_t>(other.count_));

    const int common = count_ < other.count_ ? count_ : other.count_;
    for (int i = 0; i < common; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell == b.bell) {
            out.push_back(a.beats >= b.beats ? a : b);
        } else if (a.bell < b.bell) {
            out.push_back(a);
            out.push_back(b);
        } else {
            out.push_back(b);
            out.push_back(a);
        }
    }
    for (int i = common; i < count_; ++i) {
        out.push_back(strokes_[i]);
    }
    for (int i = common; i < other.count_; ++i) {
        out.push_back(other.strokes_[i]);
    }
    return Peal(out.data(), static_cast<int>(out.size()));
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
    int total = 0;
    for (int i = 0; i < count_; ++i) {
        total += strokes_[i].beats;
    }
    return total;
}

}  // namespace rybinsk
