#include "rybinsk/peal.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int kolvo) {
    if (kolvo < 0) {
        throw PealError("Количество не может быть отрицательным");
    }
    if (kolvo == 0) {
        return;
    }
    strokes_ = new Stroke[kolvo]{};
    count_ = kolvo;
}

Peal::Peal(const Stroke* massiv_udarov, int kolvo) {
    if (kolvo < 0 || (kolvo > 0 && massiv_udarov == nullptr)) {
        throw PealError("Недопустимый массив strokes или их количество");
    }
    if (kolvo == 0) {
        return;
    }
    strokes_ = new Stroke[kolvo];
    count_ = kolvo;
    for (int i = 0; i < count_; ++i) {
        strokes_[i] = massiv_udarov[i];
    }
}

Peal::Peal(const Peal& drugoi) : Peal(drugoi.strokes_, drugoi.count_) {}

Peal::Peal(Peal&& drugoi) noexcept
    : strokes_(drugoi.strokes_), count_(drugoi.count_) {
    drugoi.strokes_ = nullptr;
    drugoi.count_ = 0;
}

Peal::~Peal() {
    delete[] strokes_;
}

Peal& Peal::operator=(const Peal& drugoi) {
    if (this == &drugoi) {
        return *this;
    }
    Peal vremenniy(drugoi);
    std::swap(strokes_, vremenniy.strokes_);
    std::swap(count_, vremenniy.count_);
    return *this;
}

Peal& Peal::operator=(Peal&& drugoi) noexcept {
    if (this == &drugoi) {
        return *this;
    }
    delete[] strokes_;
    strokes_ = drugoi.strokes_;
    count_ = drugoi.count_;
    drugoi.strokes_ = nullptr;
    drugoi.count_ = 0;
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int nomer) {
    if (nomer < 0 || nomer >= count_) {
        throw PealError("индекс вне диапазона");
    }
    return strokes_[nomer];
}

const Stroke& Peal::operator[](int nomer) const {
    if (nomer < 0 || nomer >= count_) {
        throw PealError("Индекс вне диапазона");
    }
    return strokes_[nomer];
}

Peal Peal::operator+(const Peal& drugoi) const {
    if (count_ == 0) {
        return drugoi;
    }
    if (drugoi.count_ == 0) {
        return *this;
    }

    std::vector<Stroke> itog_udary;
    itog_udary.reserve(static_cast<std::size_t>(count_ + drugoi.count_));

    int obshaya_dlina = std::min(count_, drugoi.count_);
    for (int i = 0; i < obshaya_dlina; ++i) {
        const Stroke& udar1 = strokes_[i];
        const Stroke& udar2 = drugoi.strokes_[i];
        if (udar1.bell == udar2.bell) {
            itog_udary.push_back({udar1.bell, std::max(udar1.beats, udar2.beats)});
        } else if (udar1.bell < udar2.bell) {
            itog_udary.push_back(udar1);
            itog_udary.push_back(udar2);
        } else {
            itog_udary.push_back(udar2);
            itog_udary.push_back(udar1);
        }
    }

    for (int i = obshaya_dlina; i < count_; ++i) {
        itog_udary.push_back(strokes_[i]);
    }
    for (int i = obshaya_dlina; i < drugoi.count_; ++i) {
        itog_udary.push_back(drugoi.strokes_[i]);
    }

    return Peal(itog_udary.data(), static_cast<int>(itog_udary.size()));
}

Peal& Peal::operator+=(const Peal& drugoi) {
    *this = (*this + drugoi);
    return *this;
}

bool Peal::operator==(const Peal& drugoi) const {
    if (count_ != drugoi.count_) {
        return false;
    }
    for (int i = 0; i < count_; ++i) {
        if (strokes_[i].bell != drugoi.strokes_[i].bell ||
            strokes_[i].beats != drugoi.strokes_[i].beats) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& drugoi) const {
    return !(*this == drugoi);
}

Peal::operator int() const {
    if (count_ == 0) {
        return 0;
    }
    int summa_doley = 0;
    for (int i = 0; i < count_; ++i) {
        summa_doley += strokes_[i].beats;
    }
    return summa_doley;
}

}  // namespace rybinsk
