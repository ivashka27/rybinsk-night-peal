#include "rybinsk/peal.hpp"

#include <vector>
#include <algorithm>

namespace rybinsk {


Peal::Peal() : strokes_(nullptr), count_(0) {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("error");
    }
    if (count == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }
    count_ = count;
    strokes_ = new Stroke[count_]();
}

Peal::Peal(const Stroke* p, int n) {
    if (n < 0) {
        throw PealError("error");
    }
    if (n > 0 && p == nullptr) {
        throw PealError("error");
    }
    if (n == 0) {
        strokes_ = nullptr;
        count_ = 0;
        return;
    }
    count_ = n;
    strokes_ = new Stroke[count_];
    for (int i = 0; i < n; ++i) {
        strokes_[i] = p[i];
    }
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;
    if (count_ > 0) {
        strokes_ = new Stroke[count_];
        for (int i = 0; i < count_; ++i) {
            strokes_[i] = other.strokes_[i];
        }
    } else {
        strokes_ = nullptr;
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
    if (this == &other) {
        return *this;
    }
    Stroke* new_strokes = nullptr;
    if (other.count_ > 0) {
        new_strokes = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
            new_strokes[i] = other.strokes_[i];
        }
    }
    delete[] strokes_;
    strokes_ = new_strokes;
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

Stroke& Peal::operator[](int i) {
    if (i < 0 || i >= count_) {
        throw PealError("error");
    }
    return strokes_[i];
}

const Stroke& Peal::operator[](int i) const {
    if (i < 0 || i >= count_) {
        throw PealError("error");
    }
    return strokes_[i];
}

Peal Peal::operator+(const Peal& other) const {
    int min_len = std::min(count_, other.count_);
    std::vector<Stroke> res;
    
    for (int i = 0; i < min_len; ++i) {
        const Stroke& s1 = strokes_[i];
        const Stroke& s2 = other.strokes_[i];
        
        if (s1.bell == s2.bell) {
            res.push_back({s1.bell, std::max(s1.beats, s2.beats)});
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
    
    for (int i = min_len; i < count_; ++i) {
        res.push_back(strokes_[i]);
    }
    for (int i = min_len; i < other.count_; ++i) {
        res.push_back(other.strokes_[i]);
    }
    
    return Peal(res.data(), static_cast<int>(res.size()));
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