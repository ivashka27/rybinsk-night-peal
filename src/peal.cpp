#include "rybinsk/peal.hpp"

#include <algorithm>
#include <utility>

namespace rybinsk {

    Peal::Peal() : strokes_(nullptr), count_(0) {}

    Peal::Peal(int count) : strokes_(nullptr), count_(0) {
        if (count < 0) {
            throw PealError("Count cannot be negative");
        }
        if (count > 0) {
            strokes_ = new Stroke[count]{};
            count_ = count;
        }
    }

    Peal::Peal(const Stroke* strokes, int count) : strokes_(nullptr), count_(0) {
        if (count < 0) {
            throw PealError("Count cannot be negative");
        }
        if (count > 0 && strokes == nullptr) {
            throw PealError("Strokes pointer is null with positive count");
        }
        if (count > 0) {
            strokes_ = new Stroke[count];
            for (int i = 0; i < count; ++i) {
                strokes_[i] = strokes[i];
            }
            count_ = count;
        }
    }

    Peal::Peal(const Peal& other) : strokes_(nullptr), count_(0) {
        if (other.count_ > 0) {
            strokes_ = new Stroke[other.count_];
            for (int i = 0; i < other.count_; ++i) {
                strokes_[i] = other.strokes_[i];
            }
            count_ = other.count_;
        }
    }

    Peal::Peal(Peal&& other) noexcept : strokes_(other.strokes_), count_(other.count_) {
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
            throw PealError("Index out of range");
        }
        return strokes_[index];
    }

    const Stroke& Peal::operator[](int index) const {
        if (index < 0 || index >= count_) {
            throw PealError("Index out of range");
        }
        return strokes_[index];
    }

    Peal Peal::operator+(const Peal& other) const {
        int common = std::min(count_, other.count_);
        int result_size = 0;

        for (int i = 0; i < common; ++i) {
            if (strokes_[i].bell == other.strokes_[i].bell) {
                result_size += 1;
            }
            else {
                result_size += 2;
            }
        }
        result_size += (count_ - common) + (other.count_ - common);

        Peal result(result_size);
        int pos = 0;

        for (int i = 0; i < common; ++i) {
            const Stroke& a = strokes_[i];
            const Stroke& b = other.strokes_[i];
            if (a.bell == b.bell) {
                result.strokes_[pos++] = Stroke{ a.bell, std::max(a.beats, b.beats) };
            }
            else if (a.bell < b.bell) {
                result.strokes_[pos++] = a;
                result.strokes_[pos++] = b;
            }
            else {
                result.strokes_[pos++] = b;
                result.strokes_[pos++] = a;
            }
        }

        for (int i = common; i < count_; ++i) {
            result.strokes_[pos++] = strokes_[i];
        }
        for (int i = common; i < other.count_; ++i) {
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

    Peal::operator int() const
    {
        int sum = 0;
        for (int i = 0; i < count_; ++i) {
            sum += strokes_[i].beats;
        }
        return sum;
    }

} // namespace rybinsk
