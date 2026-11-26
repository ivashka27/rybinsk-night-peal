#include "rybinsk/peal.hpp"
#include <vector>

namespace rybinsk {
//namespace {
//
//[[noreturn]] void notReady() {
//    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
//    throw std::logic_error("ещё не написано");
//}
//
//}  // namespace

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Отрицательный размер листка");
    }

    if (count > 0) {
        strokes_ = new Stroke[count]{};
    }

    count_ = count;
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("Неверные данные листка");
    }

    if (count > 0) {
        strokes_ = new Stroke[count];
        for (int i = 0; i < count; i++) {
            strokes_[i] = strokes[i];
        }
    }

    count_ = count;
}

Peal::Peal(const Peal& other) {
    if (other.count_ > 0) {
        strokes_ = new Stroke[other.count_];
        for (int i = 0; i < other.count_; i++) {
            strokes_[i] = other.strokes_[i];
        }
    }

    count_ = other.count_;
}

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

    Peal copy(other);

    delete[] strokes_;
    strokes_ = copy.strokes_;
    count_ = copy.count_;

    copy.strokes_ = nullptr;
    copy.count_ = 0;

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
        throw PealError("Неверный номер удара");
    }

    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Неверный номер удара");
    }

    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    std::vector<Stroke> result;

    int common = count_ < other.count_ ? count_ : other.count_;

    for (int i = 0; i < common; i++) {
        Stroke a = strokes_[i];
        Stroke b = other.strokes_[i];

        if (a.bell == b.bell) {
            if (a.beats >= b.beats) {
                result.push_back(a);
            } else {
                result.push_back(b);
            }
        } else {
            if (a.bell < b.bell) {
                result.push_back(a);
                result.push_back(b);
            } else {
                result.push_back(b);
                result.push_back(a);
            }
        }
    }

    for (int i = common; i < count_; i++) {
        result.push_back(strokes_[i]);
    }

    for (int i = common; i < other.count_; i++) {
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

    for (int i = 0; i < count_; i++) {
        sum += strokes_[i].beats;
    }

    return sum;
}

}  // namespace rybinsk
