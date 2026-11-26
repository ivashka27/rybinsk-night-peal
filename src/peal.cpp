#include "rybinsk/peal.hpp"

namespace rybinsk {
namespace {

[[noreturn]] void notReady() {
    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
    throw std::logic_error("ещё не написано");
}

}  // namespace

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("отриц колво ударов");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]();
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("отриц колво ударов");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("пустой указатель при count > 0");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        count_ = count;
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ > 0) {
        strokes_ = new Stroke[other.count_];
        count_ = other.count_;
        for (int i = 0; i < count_; ++i) {
            strokes_[i] = other.strokes_[i];
        }
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
    // Освободите strokes_. delete[] nullptr вызывать можно.
        delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this != &other) {
        *this = Peal(other);
    }
    return *this;
}

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
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
        throw PealError("номер удара вне диапазона");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("номер удара вне диапазона");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    int common = (count_ < other.count_) ? count_ : other.count_;

    int total = 0;
    for (int i = 0; i < common; ++i) {
        total += (strokes_[i].bell == other.strokes_[i].bell) ? 1 : 2;
    }
    total += (count_ - common) + (other.count_ - common);

    Peal result(total);
    int k = 0;
    for (int i = 0; i < common; ++i) {
        const Stroke& x = strokes_[i];
        const Stroke& y = other.strokes_[i];

        if (x.bell == y.bell) {
            result.strokes_[k++] = (x.beats >= y.beats) ? x : y;
        } else if (x.bell < y.bell) {
            result.strokes_[k++] = x;
            result.strokes_[k++] = y;
        } else {
            result.strokes_[k++] = y;
            result.strokes_[k++] = x;
        }
    }

    for (int i = common; i < count_; ++i) {
        result.strokes_[k++] = strokes_[i];
    }
    for (int i = common; i < other.count_; ++i) {
        result.strokes_[k++] = other.strokes_[i];
    }

    return result;

}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal&other) const {
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

bool Peal::operator!=(const Peal&other) const {
    return !(*this == other);
}

Peal::operator int() const {
    if (count_ == 0) {
        return 0;
    }
    int sum = 0;
    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }
    return sum;
}

}  // namespace rybinsk
