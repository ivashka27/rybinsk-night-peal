#include "rybinsk/peal.hpp"
#include <vector>
namespace rybinsk {


Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count == 0) {
        return;
    }
    count_ = count;
    strokes_ = new Stroke[count_];
    for (int i = 0; i < count_; ++i) {
        strokes_[i].bell = 0;
        strokes_[i].beats = 0;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("null pointer");
    }
    if (count == 0) {
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
        return;
    }
    count_ = other.count_;
    strokes_ = new Stroke[count_];
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = other.strokes_[i];
    }
}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept {
    strokes_ = other.strokes_;
    count_ = other.count_;
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

    Stroke* newStrokes = nullptr;
    if (other.count_ > 0) {
        newStrokes = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
            newStrokes[i] = other.strokes_[i];
        }
    }

    delete[] strokes_;
    strokes_ = newStrokes;
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
    std::vector<Stroke> res;

    int minCount = count_;
    if (other.count_ < minCount) {
        minCount = other.count_;
    }

    for (int i = 0; i < minCount; ++i) {
        Stroke a = strokes_[i];
        Stroke b = other.strokes_[i];

        if (a.bell == b.bell) {
            if (a.beats >= b.beats) {
                res.push_back(a);
            } else {
                res.push_back(b);
            }
        } else if (a.bell < b.bell) {
            res.push_back(a);
            res.push_back(b);
        } else {
            res.push_back(b);
            res.push_back(a);
        }
    }

    for (int i = minCount; i < count_; ++i) {
        res.push_back(strokes_[i]);
    }
    for (int i = minCount; i < other.count_; ++i) {
        res.push_back(other.strokes_[i]);
    }

    if (res.empty()) {
        return Peal();
    }
    return Peal(res.data(), (int)res.size());
}

Peal& Peal::operator+=(const Peal& other) {
    Peal sum = *this + other;
    *this = sum;
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
