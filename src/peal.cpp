#include "rybinsk/peal.hpp"

#include <vector>

namespace rybinsk {

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0) {
        throw PealError("count cannot be negative");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)) {
        throw PealError("invalid strokes or count");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
        count_ = count;
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ > 0 && other.strokes_ != nullptr) {
        strokes_ = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
            strokes_[i] = other.strokes_[i];
        }
        count_ = other.count_;
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
    strokes_ = nullptr;
    count_ = 0;
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other) {
        return *this;
    }
    Stroke* new_strokes = nullptr;
    if (other.count_ > 0 && other.strokes_ != nullptr) {
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

Stroke& Peal::operator[](int index) {
    if (index < 0 || index >= count_) {
        throw PealError("index out of bounds");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) {
        throw PealError("index out of bounds");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    if (count_ == 0) {
        return other;
    }
    if (other.count_ == 0) {
        return *this;
    }

    std::vector<Stroke> result;
    const int common_count = count_ < other.count_ ? count_ : other.count_;
    for (int i = 0; i < common_count; ++i) {
        const Stroke& s1 = strokes_[i];
        const Stroke& s2 = other.strokes_[i];
        if (s1.bell == s2.bell) {
            const int max_beats = s1.beats > s2.beats ? s1.beats : s2.beats;
            result.push_back(Stroke{s1.bell, max_beats});
        } else if (s1.bell < s2.bell) {
            result.push_back(s1);
            result.push_back(s2);
        } else {
            result.push_back(s2);
            result.push_back(s1);
        }
    }

    for (int i = common_count; i < count_; ++i) {
        result.push_back(strokes_[i]);
    }
    for (int i = common_count; i < other.count_; ++i) {
        result.push_back(other.strokes_[i]);
    }

    return Peal(result.data(), static_cast<int>(result.size()));
}

Peal& Peal::operator+=(const Peal& other) {
    if (other.count_ == 0) {
        return *this;
    }
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
    int total_beats = 0;
    for (int i = 0; i < count_; ++i) {
        total_beats += strokes_[i].beats;
    }
    return total_beats;
}

}  // namespace rybinsk
