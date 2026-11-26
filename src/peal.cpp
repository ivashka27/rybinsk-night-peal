#include "rybinsk/peal.hpp"

namespace rybinsk {
namespace {

[[noreturn]] void notReady() {
    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
    throw std::logic_error("ещё не написано");
}

}  // namespace

Peal::Peal() : strokes_(nullptr), count_(0) {}

Peal::Peal(int count) : strokes_(nullptr), count_(0) {
    if (count < 0 ){
        throw PealError("negative count");
    }
    if (count > 0){
        strokes_ = new Stroke[count]{};
        count_ = count;
    }

}

Peal::Peal(const Stroke* strokes, int count)
    : strokes_(nullptr), count_(0) {
    if (count < 0) {
        throw PealError("negative count");
    }
    if (count > 0 && strokes == nullptr) {
        throw PealError("null strokes");
    }

    if (count > 0) {
        strokes_ = new Stroke[count]{};
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
        count_ = count;
    }
}

Peal::Peal(const Peal& other)
    : Peal(other.strokes_, other.count_) {}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept
    : strokes_(other.strokes_), count_(other.count_) {
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
    const int paired = count_ < other.count_ ? count_ : other.count_;
    int resultCount = count_ + other.count_;

    for (int i = 0; i < paired; ++i) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            --resultCount;
        }
    }

    Peal result(resultCount);
    int pos = 0;

    for (int i = 0; i < paired; ++i) {
        const Stroke& left = strokes_[i];
        const Stroke& right = other.strokes_[i];

        if (left.bell == right.bell) {
            result.strokes_[pos++] =
                left.beats >= right.beats ? left : right;
        } else if (left.bell < right.bell) {
            result.strokes_[pos++] = left;
            result.strokes_[pos++] = right;
        } else {
            result.strokes_[pos++] = right;
            result.strokes_[pos++] = left;
        }
    }

    for (int i = paired; i < count_; ++i) {
        result.strokes_[pos++] = strokes_[i];
    }
    for (int i = paired; i < other.count_; ++i) {
        result.strokes_[pos++] = other.strokes_[i];
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
    int total = 0;
    for (int i = 0; i < count_; ++i) {
        total += strokes_[i].beats;
    }
    return total;
}

}  // namespace rybinsk
