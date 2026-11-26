#include "rybinsk/peal.hpp"
#include <utility>
namespace rybinsk {
Peal::Peal() {}
Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Negative count");}

    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}
Peal::Peal(const Stroke* strokes, int count) : Peal(count) {
    if (strokes == nullptr && count > 0) {
        throw PealError("Null pointer");
    }
    for (int i = 0; i < count; ++i) {
        strokes_[i] = strokes[i];
    }
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
    if (index < 0 || index >= count_) {
        throw PealError("Invalid index");
    }

    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Invalid index");
    }

    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    int common = count_ < other.count_
        ? count_ : other.count_;
    int resultCount = count_ + other.count_;
    for (int i = 0; i < common; i++) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            --resultCount;
        }
    }

    Peal result(resultCount);
    int pos = 0;
    for (int i = 0; i < common; i++) {
        Stroke a = strokes_[i];
        Stroke b = other.strokes_[i];

        if (a.bell == b.bell) {
            result[pos++] = a.beats >= b.beats ? a : b;
        } else if (a.bell < b.bell) {
            result[pos++] = a;
            result[pos++] = b;
        } else {
            result[pos++] = b;
            result[pos++] = a;
        }
    }

    for (int i = common; i < count_; i++) {
        result[pos++] = strokes_[i];
    }
    for (int i = common; i < other.count_; i++) {
        result[pos++] = other.strokes_[i];}
    return result;}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) {
        return false;
    }

    for (int i = 0; i < count_; i++) {
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
