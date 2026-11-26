#include "rybinsk/peal.hpp"

#include <vector>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Отрицательная длина");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (count > 0 && strokes == nullptr)) {
        throw PealError("Некорректный массив");
    }
    if (count == 0) {
        return;
    }

    strokes_ = new Stroke[count];
    count_ = count;
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = strokes[i];
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

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
    if (this != &other) {
        *this = Peal(other);
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
        throw PealError("Неверный индекс");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Неверный индекс");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    std::vector<Stroke> result;
    int pairs = count_;
    if (other.count_ < pairs) {
        pairs = other.count_;
    }

    for (int i = 0; i < pairs; ++i) {
        Stroke left = strokes_[i];
        Stroke right = other.strokes_[i];

        if (left.bell < right.bell) {
            result.push_back(left);
            result.push_back(right);
        } else if (left.bell > right.bell) {
            result.push_back(right);
            result.push_back(left);
        } else if (left.beats >= right.beats) {
            result.push_back(left);
        } else {
            result.push_back(right);
        }
    }

    for (int i = pairs; i < count_; ++i) {
        result.push_back(strokes_[i]);
    }
    for (int i = pairs; i < other.count_; ++i) {
        result.push_back(other.strokes_[i]);
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
    long long sum = 0;
    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }
    return static_cast<int>(sum);
}

}  // namespace rybinsk