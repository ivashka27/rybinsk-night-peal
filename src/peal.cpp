#include "rybinsk/peal.hpp"
#include <algorithm>
#include <utility>
namespace rybinsk {
Peal::Peal() : strokes_(nullptr), count_(0) {}
Peal::Peal(int count) : strokes_(nullptr), count_(0) {
    if (count < 0) {
        throw PealError("count cannot be negative");
    }
    if (count > 0) {
        strokes_ = new Stroke[count]{};
        count_ = count;
    }
}
Peal::Peal(const Stroke* strokes, int count) : Peal(count) {
    if (count > 0) {
        if (strokes == nullptr) {
            delete[] strokes_;
            strokes_ = nullptr;
            count_ = 0;
            throw PealError("strokes pointer cannot be null when count > 0");
        }
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
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
    if (this == &other) {
        return *this;
    }
    Peal temp(other);
    *this = std::move(temp);
    return *this;
}
Peal& Peal::operator=(Peal&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] strokes_;
    strokes_ = std::exchange(other.strokes_, nullptr);
    count_ = std::exchange(other.count_, 0);
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
    if (count_ == 0) {
        return other;
    }
    if (other.count_ == 0) {
        return *this;
    }
    const int min_len = std::min(count_, other.count_);
    const int max_len = std::max(count_, other.count_);
    int total_len = 0;
    for (int i = 0; i < min_len; ++i) {
        if (strokes_[i].bell == other.strokes_[i].bell) {
            total_len += 1;
        } else {
            total_len += 2;
        }
    }
    total_len += (max_len - min_len);
    Peal result(total_len);
    int pos = 0;
    for (int i = 0; i < min_len; ++i) {
        const Stroke& a = strokes_[i];
        const Stroke& b = other.strokes_[i];
        if (a.bell == b.bell) {
            result.strokes_[pos++] = (b.beats > a.beats) ? b : a;
        } else if (a.bell < b.bell) {
            result.strokes_[pos++] = a;
            result.strokes_[pos++] = b;
        } else {
            result.strokes_[pos++] = b;
            result.strokes_[pos++] = a;
        }
    }
    const Peal& longer = (count_ > other.count_) ? *this : other;
    for (int i = min_len; i < max_len; ++i) {
        result.strokes_[pos++] = longer.strokes_[i];
    }
    return result;
}
Peal& Peal::operator+=(const Peal& other) {
    Peal combined = *this + other;
    *this = std::move(combined);
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
    int sum = 0;
    for (int i = 0; i < count_; ++i) {
        sum += strokes_[i].beats;
    }
    return sum;
}
}  // namespace rybinsk
