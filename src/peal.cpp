#include "rybinsk/peal.hpp"

#include <utility>

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
    throw PealError("отрицательный размер");
   }

   if (count > 0) {
    strokes_ = new Stroke[count]{};
    count_ = count;
   }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("отрицательный размер");
    }

    if (strokes == nullptr && count > 0) {
        throw PealError("пустой указатель");
    }

    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;

        for (int i = 0; i < count_; ++i) {
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) {
    if (other.count_ > 0) {
        strokes_ = new Stroke[other.count_]{};
        count_ = other.count_;

        for (int i = 0; i < count_; ++i) {
            strokes_[i] = other.strokes_[i];
        }
    }
}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
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

    Stroke* new_strokes = nullptr;

    if (other.count_ > 0) {
        new_strokes = new Stroke[other.count_]{};

        for (int i = 0; i < other.count_; ++i) {
            new_strokes[i] = other.strokes_[i];
        }
    }

    delete[] strokes_;

    strokes_ = new_strokes;
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

Stroke& Peal::operator[](int i) {
    if (i < 0 || i >= count_) {
        throw PealError("плохой индекс");
    }

    return strokes_[i];
}

const Stroke& Peal::operator[](int i) const {
    if (i < 0 || i >= count_) {
        throw PealError("плохой индекс");
    }

    return strokes_[i];
}

Peal Peal::operator+(const Peal& other) const {
    int pair_count = count_ < other.count_ ? count_ : other.count_;
    int res_count = 0;

    for (int i = 0; i < pair_count; ++i) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            res_count++;
        } else {
            res_count += 2;
        }
    }

    res_count += count_ - pair_count;
    res_count += other.count_ - pair_count;

    Peal result(res_count);
    int out = 0;

    for (int i = 0; i < pair_count; ++i) {
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];

        if (left.bell < right.bell) {
            result[out] = left;
            ++out;

            result[out] = right;
            ++out;
        } else if (left.bell > right.bell) {
            result[out] = right;
            ++out;

            result[out] = left;
            ++out;
        } else {
            if (left.beats >= right.beats) {
                result[out] = left;
            } else {
                result[out] = right;
            }
            ++out;
        }
    }

    for (int i = pair_count; i < count_; ++i) {
        result[out] = strokes_[i];
        ++out;
    }

    for (int i = pair_count; i < other.count_; ++i) {
        result[out] = other.strokes_[i];
        ++out;
    }
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    Peal result = *this + other;

    *this = std::move(result);

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
    if (*this == other) {
        return false;
    }
    return true;

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
