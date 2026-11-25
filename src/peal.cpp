#include "rybinsk/peal.hpp"

namespace rybinsk {

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int count) {
    if (count < 0) throw PealError("negative count");
    count_ = count;
    if (count == 0) {
        strokes_ = nullptr;
    } else {
        strokes_ = new Stroke[count];
        for (int i = 0; i < count; i++) {
            strokes_[i].bell = 0;
            strokes_[i].beats = 0;
        }
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) throw PealError("negative count");
    if (count > 0 && strokes == nullptr) throw PealError("null pointer");
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
    if (this == &other) return *this;
    Stroke* fresh = nullptr;
    if (other.count_ > 0) {
        fresh = new Stroke[other.count_];
        for (int i = 0; i < other.count_; i++) {
            fresh[i] = other.strokes_[i];
        }
    }
    delete[] strokes_;
    strokes_ = fresh;
    count_ = other.count_;
    return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
    if (this == &other) return *this;
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
    if (index < 0 || index >= count_) throw PealError("index out of range");
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_) throw PealError("index out of range");
    return strokes_[index];
}

Peal::operator int() const {
    int sum = 0;
    for (int i = 0; i < count_; i++) {
        sum += strokes_[i].beats;
    }
    return sum;
}

Peal Peal::operator+(const Peal& other) const {
    int maxSize = size() + other.size();
    Stroke* buf = nullptr;
    if (maxSize > 0) buf = new Stroke[maxSize];

    int minLen = size();
    if (other.size() < minLen) minLen = other.size();

    int k = 0;
    for (int i = 0; i < minLen; i++) {
        Stroke sa = strokes_[i];
        Stroke sb = other.strokes_[i];
        if (sa.bell == sb.bell) {
            if (sa.beats >= sb.beats) buf[k++] = sa;
            else                      buf[k++] = sb;
        } else if (sa.bell < sb.bell) {
            buf[k++] = sa;
            buf[k++] = sb;
        } else {
            buf[k++] = sb;
            buf[k++] = sa;
        }
    }
    for (int i = minLen; i < size(); i++) buf[k++] = strokes_[i];
    for (int i = minLen; i < other.size(); i++) buf[k++] = other.strokes_[i];

    Peal result(buf, k);
    delete[] buf;
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    Peal sum = (*this) + other;
    *this = sum;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_) return false;
    for (int i = 0; i < count_; i++) {
        if (strokes_[i].bell  != other.strokes_[i].bell)  return false;
        if (strokes_[i].beats != other.strokes_[i].beats) return false;
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return !(*this == other);
}

} // namespace rybinsk