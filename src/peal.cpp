#include "rybinsk/peal.hpp"

#include <vector>

namespace rybinsk {

    Peal::Peal() {}

    Peal::Peal(int count) {
        if (count < 0) {
            throw PealError("Peal: отрицательная длина");
        }
        if (count == 0) {
            return;
        }
        strokes_ = new Stroke[count]{};
        count_ = count;
    }

    Peal::Peal(const Stroke* strokes, int count) {
        if (count < 0) {
            throw PealError("Peal: отрицательная длина");
        }
        if (count == 0) {
            return;
        }
        if (strokes == nullptr) {
            throw PealError("Peal: nullptr при положительной длине");
        }
        strokes_ = new Stroke[count];
        for (int i = 0; i < count; ++i) {
            strokes_[i] = strokes[i];
        }
        count_ = count;
    }

    Peal::Peal(const Peal& other) {
        if (other.count_ == 0) {
            return;
        }
        strokes_ = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i) {
            strokes_[i] = other.strokes_[i];
        }
        count_ = other.count_;
    }

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
        Stroke* fresh = nullptr;
        if (other.count_ > 0) {
            fresh = new Stroke[other.count_];
            for (int i = 0; i < other.count_; ++i) {
                fresh[i] = other.strokes_[i];
            }
        }
        delete[] strokes_;
        strokes_ = fresh;
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

    Stroke& Peal::operator[](int index) {
        if (index < 0 || index >= count_) {
            throw PealError("Peal: индекс вне диапазона");
        }
        return strokes_[index];
    }

    const Stroke& Peal::operator[](int index) const {
        if (index < 0 || index >= count_) {
            throw PealError("Peal: индекс вне диапазона");
        }
        return strokes_[index];
    }

    Peal Peal::operator+(const Peal& other) const {
        const int minLen = count_ < other.count_ ? count_ : other.count_;
        std::vector<Stroke> buf;
        buf.reserve(static_cast<std::size_t>(count_ + other.count_));

        for (int i = 0; i < minLen; ++i) {
            const Stroke& a = strokes_[i];
            const Stroke& b = other.strokes_[i];
            if (a.bell == b.bell) {
                buf.push_back(a.beats >= b.beats ? a : b);
            }
            else if (a.bell < b.bell) {
                buf.push_back(a);
                buf.push_back(b);
            }
            else {
                buf.push_back(b);
                buf.push_back(a);
            }
        }

        const Peal* longer = count_ > other.count_ ? this : &other;
        for (int i = minLen; i < longer->count_; ++i) {
            buf.push_back(longer->strokes_[i]);
        }

        return Peal(buf.data(), static_cast<int>(buf.size()));
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
        for (int i = 0; i < count_; ++i) {
            sum += strokes_[i].beats;
        }
        return sum;
    }

}  // namespace rybinsk
