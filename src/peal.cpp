#include "rybinsk/peal.hpp"
#include <vector>
#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int n) {
    if (n < 0) {
        throw PealError("число ударов < 0");
    }
    if (n == 0) {
        return;
    }
    strokes_ = new Stroke[n]{};
    count_ = n;
}

Peal::Peal(const Stroke* strokes, int n) {
    if (n < 0) {
        throw PealError("число ударов < 0");
    }
    if (n > 0 && strokes == nullptr) {
        throw PealError("пустой указатель при числе ударов > 0");
    }
    if (n == 0) {
        return;
    }
    strokes_ = new Stroke[n];
    count_ = n;
    for (int i = 0; i < n; i++) {
        strokes_[i] = strokes[i];
    }

}

Peal::Peal(const Peal& df) : Peal(df.strokes_, df.count_) {
}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& df) noexcept {
    strokes_ = df.strokes_;
    count_ = df.count_;

    df.strokes_ = nullptr;
    df.count_ = 0;
}

Peal::~Peal() {
    delete[] strokes_;
    // Освободите strokes_. delete[] nullptr вызывать можно.
}

Peal& Peal::operator=(const Peal& df ) {
    if (this == &df) {
        return *this;
    }
    Peal copy(df);
    delete[] strokes_;
    strokes_ = copy.strokes_;
    count_ = copy.count_;
    copy.strokes_ = nullptr;
    copy.count_ = 0;
    return *this;
}

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
Peal& Peal::operator=(Peal&& df) noexcept {
    if (this == &df) {
        return *this;
    }
    delete[] strokes_;
    strokes_ = df.strokes_;
    count_ = df.count_;
    df.strokes_ = nullptr;
    df.count_ = 0;
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int i) {
    if (i < 0 || i >= count_) {
        throw PealError("удар выходдит за грницы листа");
    }
    return strokes_[i];
}

const Stroke& Peal::operator[](int i) const {
    if (i < 0 || i >= count_) {
        throw PealError("удар выходдит за грницы листа");
    }
    return strokes_[i];
}

Peal Peal::operator+(const Peal& df) const {
    std::vector<Stroke> summary;
    const int paired = count_<df.count_ ? count_ : df.count_; summary.reserve(static_cast<size_t>(count_ + df.count_));

    for (int i = 0; i < paired; i++) {
        const Stroke& l = strokes_[i];
        const Stroke& r = df.strokes_[i];
        if (l.bell != r.bell) {
            if (l.bell < r.bell) {
                summary.push_back(l);
                summary.push_back(r);
            } else {
                summary.push_back(r);
                summary.push_back(l);
            }
        } else if (l.beats >= r.beats){
            summary.push_back(l);
        } else {
            summary.push_back(r);
        }
    }
    for (int i = paired; i < count_; i++) {
        summary.push_back(strokes_[i]);
    }
    for (int i = paired; i < df.count_; i++) {
        summary.push_back(df.strokes_[i]);
    }
    return Peal(summary.data(), static_cast<int>(summary.size()));
}

Peal& Peal::operator+=(const Peal& df) {
    Peal summary = *this + df;
    *this = std::move(summary);
    return *this;
}

bool Peal::operator==(const Peal& df) const {
    if (count_ != df.count_) {
        return false;
    }
    for (int i = 0; i < count_; i++) {
        if (strokes_[i].bell != df.strokes_[i].bell ||
        strokes_[i].beats != df.strokes_[i].beats){
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& df) const {
    return !(*this == df);
}

Peal::operator int() const {
    int sum_of_beats = 0;
    for (int i = 0; i < count_; i++) {
        sum_of_beats += strokes_[i].beats;
    }
    return sum_of_beats;
}

}  // namespace rybinsk
