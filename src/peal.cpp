#include "rybinsk/peal.hpp"

namespace rybinsk {

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("Wrong count");
    }

    count_ = count;

    if (count == 0) {
        strokes_ = nullptr;
    } else {
        strokes_ = new Stroke[count]{};
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (count > 0 && strokes == nullptr)) {
        throw PealError("Wrong array");
    }

    count_ = count;

    if (count == 0) {
        strokes_ = nullptr;
    } else {
        strokes_ = new Stroke[count];

        for (int i = 0; i < count; i++) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;

    if (count_ == 0) {
        strokes_ = nullptr;
    } else {
        strokes_ = new Stroke[count_];

        for (int i = 0; i < count_; i++) {
            strokes_[i] = other.strokes_[i];
        }
    }
}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept {
    count_ = other.count_;
    strokes_ = other.strokes_;

    other.count_ = 0;
    other.strokes_ = nullptr;
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

        for (int i = 0; i < other.count_; i++) {
            newStrokes[i] = other.strokes_[i];
        }
    }

    delete[] strokes_;

    strokes_ = newStrokes;
    count_ = other.count_;

    return *this;
}

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
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
        throw PealError("Wrong index");
    }

    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("Wrong index");
    }

    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    int count = count_ + other.count_;

    int i = 0;

    while (i < count_ && i < other.count_) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            count--;
        }
        i++;
    }

    Peal result(count);
    int j = 0;
    i = 0;

    while (i < count_ && i < other.count_) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            if (strokes_[i].beats >= other.strokes_[i].beats) {
                result[j] = strokes_[i];
            } else {
                result[j] = other.strokes_[i];
            }
            j++;
        } else if (strokes_[i].bell < other.strokes_[i].bell) {
            result[j] = strokes_[i];
            j++;
            result[j] = other.strokes_[i];
            j++;
        } else {
            result[j] = other.strokes_[i];
            j++;
            result[j] = strokes_[i];
            j++;
        }

        i++;
    }

    for (int k = i; k < count_; k++) {
        result[j] = strokes_[k];
        j++;
    }

    for (int k = i; k < other.count_; k++) {
        result[j] = other.strokes_[k];
        j++;
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

    for (int i = 0; i < count_; i++) {
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
    int sum = 0;

    for (int i = 0; i < count_; i++) {
        sum += strokes_[i].beats;
    }

    return sum;
}

}  // namespace rybinsk
