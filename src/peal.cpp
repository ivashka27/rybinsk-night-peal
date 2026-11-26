#include "rybinsk/peal.hpp"

#include <algorithm>
#include <limits>
#include <utility>

namespace rybinsk {
namespace {

void checkIndex(int index, int count) {
    if (index < 0 || index >= count) {
        throw PealError("Номер удара за пределами листка");
    }
}

int checkedInt(long long value) {
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
        throw PealError("Результат не помещается в int");
    }
    return static_cast<int>(value);
}

}  // namespace

Peal::Peal() = default;

Peal::Peal(int count) : count_(count) {
    if (count < 0) {
        throw PealError("Отрицательное число ударов");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
    }
}

Peal::Peal(const Stroke* strokes, int count) : count_(count) {
    if (count < 0 || (count > 0 && strokes == nullptr)) {
        throw PealError("Некорректный массив ударов");
    }
    if (count > 0) {
        strokes_ = new Stroke[count];
        std::copy_n(strokes, count, strokes_);
    }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

Peal::Peal(Peal&& other) noexcept
    : strokes_(std::exchange(other.strokes_, nullptr)),
      count_(std::exchange(other.count_, 0)) {}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
    if (this != &other) {
        Peal copy(other);
        *this = std::move(copy);
    }
    return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
    if (this != &other) {
        delete[] strokes_;
        strokes_ = std::exchange(other.strokes_, nullptr);
        count_ = std::exchange(other.count_, 0);
    }
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int index) {
    checkIndex(index, count_);
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    checkIndex(index, count_);
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    const int pairs = std::min(count_, other.count_);
    const Peal& longer = count_ >= other.count_ ? *this : other;


    long long resultSize = longer.size();
    for (int i = 0; i < pairs; ++i) {
        if (strokes_[i].bell != other.strokes_[i].bell) {
            ++resultSize;
        }
    }

    Peal result(checkedInt(resultSize));
    int next = 0;
    for (int i = 0; i < pairs; ++i) {
        Stroke first = strokes_[i];
        Stroke second = other.strokes_[i];
        if (first.bell == second.bell) {
            result[next++] = {first.bell, std::max(first.beats, second.beats)};
        } else {
            if (first.bell > second.bell) {
                std::swap(first, second);
            }
            result[next++] = first;
            result[next++] = second;
        }
    }
    for (int i = pairs; i < longer.size(); ++i) {
        result[next++] = longer[i];
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
    long long total = 0;
    for (int i = 0; i < count_; ++i) {
        total += strokes_[i].beats;
    }
    return checkedInt(total);
}

}  // namespace rybinsk
