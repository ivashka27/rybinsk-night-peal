#include "rybinsk/peal.hpp"

namespace rybinsk {
namespace {

[[noreturn]] void notReady() {
    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
    throw std::logic_error("ещё не написано");
}

}  // namespace

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int n) {
    if(n < 0) {
        throw PealError("Invalid count");
    }
    count_ = n;
    strokes_ = nullptr;
    if(n > 0) {
        strokes_ = new Stroke[n]{};
    }
}

Peal::Peal(const Stroke* a, int n) {
    if(n < 0 || (n > 0 && a == nullptr)) {
        throw PealError("Invalid array");
    }
    count_ = n;
    strokes_ = nullptr;
    if(n > 0) {
        strokes_ = new Stroke[n];
        for(int i = 0; i < n; i++) {
            strokes_[i] = a[i];
        }
    }
}

Peal::Peal(const Peal& a) {
    count_ = a.count_;
    strokes_ = nullptr;
    if(count_ > 0) {
        strokes_ = new Stroke[count_];
        for(int i = 0; i < count_; i++) {
            strokes_[i] = a.strokes_[i];
        }
    }
}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& a) noexcept {
    count_ = a.count_;
    strokes_ = a.strokes_;
    a.count_ = 0;
    a.strokes_ = nullptr;
}

Peal::~Peal() {
    // Освободите strokes_. delete[] nullptr вызывать можно.
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& a) {
    if(this != &a) {
        Stroke* b = nullptr;
        if(a.count_ > 0) {
            b = new Stroke[a.count_];
            for(int i = 0; i < a.count_; i++) {
                b[i] = a.strokes_[i];
            }
        }
        delete[] strokes_;
        strokes_ = b;
        count_ = a.count_;
    }
    return *this;
}

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
Peal& Peal::operator=(Peal&& a) noexcept {
    if(this != &a) {
        delete[] strokes_;
        strokes_ = a.strokes_;
        count_ = a.count_;
        a.strokes_ = nullptr;
        a.count_ = 0;
    }
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int i) {
    if(i < 0 || i >= count_) {
        throw PealError("Invalid index");
    }
    return strokes_[i];
}

const Stroke& Peal::operator[](int i) const {
    if(i < 0 || i >= count_) {
        throw PealError("Invalid index");
    }
    return strokes_[i];
}

Peal Peal::operator+(const Peal& a) const {
    int n = count_ + a.count_;
    for(int i = 0; i < count_ && i < a.count_; i++) {
        if(strokes_[i].bell == a.strokes_[i].bell) {
            n--;
        }
    }
    Peal b(n);
    int i = 0, k = 0;
    while(i < count_ && i < a.count_) {
        if(strokes_[i].bell == a.strokes_[i].bell) {
            if(strokes_[i].beats >= a.strokes_[i].beats) {
                b[k++] = strokes_[i];
            } else {
                b[k++] = a.strokes_[i];
            }
        } else if(strokes_[i].bell < a.strokes_[i].bell) {
            b[k++] = strokes_[i];
            b[k++] = a.strokes_[i];
        } else {
            b[k++] = a.strokes_[i];
            b[k++] = strokes_[i];
        }
        i++;
    }
    for(int j = i; j < count_; j++) {
        b[k++] = strokes_[j];
    }
    for(int j = i; j < a.count_; j++) {
        b[k++] = a.strokes_[j];
    }
    return b;
}

Peal& Peal::operator+=(const Peal& a) {
    *this = *this + a;
    return *this;
}

bool Peal::operator==(const Peal& a) const {
    if(count_ != a.count_) {
        return false;
    }
    for(int i = 0; i < count_; i++) {
        if(strokes_[i].bell != a.strokes_[i].bell || strokes_[i].beats != a.strokes_[i].beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& a) const {
    return !(*this == a);
}

Peal::operator int() const {
    if (count_ == 0) {
        return 0;
    }
    int sum = 0;
    for(int i = 0; i < count_; i++) {
        sum += strokes_[i].beats;
    }
    return sum;
}

}  // namespace rybinsk