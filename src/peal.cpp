#include "rybinsk/peal.hpp"
#include <vector>

namespace rybinsk {
namespace {

[[noreturn]] void notReady() {
    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
    throw std::logic_error("ещё не написано");
}

} 

Peal::Peal() {}

Peal::Peal(int count) {
    if (count < 0){
        throw PealError("Ошибка");
    }
    count_ = count;
    if (count > 0){
        strokes_ = new Stroke[count_]{};
    }
}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0 || (strokes == nullptr && count > 0)){
        throw PealError("Wrong");
    }
    count_ = count;
    if (count > 0){
        strokes_ = new Stroke[count_];
        for (int i = 0; i < count_; ++i){
            strokes_[i] = strokes[i];
        }
    }
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;
    if (count_ > 0){
        strokes_ = new Stroke[count_];
        for (int i = 0; i < count_; ++i){
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
    if (this == &other){
        return *this;
    }
    Stroke* newStrokes = nullptr;
    if (other.count_ > 0){
        newStrokes = new Stroke[other.count_];
        for (int i = 0; i < other.count_; ++i){
            newStrokes[i] = other.strokes_[i];
        }
    }
    delete[] strokes_;
    strokes_ = newStrokes;
    count_ = other.count_;
    return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
    if (this == &other){
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
    if (index < 0 || index >= count_){
        throw PealError("Wrong");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 || index >= count_){
        throw PealError("Wrong");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    std::vector<Stroke> res;
    int n = count_;
    if (other.count_ < n){
        n = other.count_;
    }
    for (int i = 0; i < n; ++i){
        if (strokes_[i].bell == other.strokes_[i].bell){
            if (strokes_[i].beats >= other.strokes_[i].beats){
                res.push_back(strokes_[i]);
            }else{
                res.push_back(other.strokes_[i]);
            }
        }else{
            if (strokes_[i].bell < other.strokes_[i].bell){
                res.push_back(strokes_[i]);
                res.push_back(other.strokes_[i]);
            }else{
                res.push_back(other.strokes_[i]);
                res.push_back(strokes_[i]);
            }
        }
    }
    for (int i = n; i < count_; ++i){
        res.push_back(strokes_[i]);
    }
    for (int i = n; i < other.count_; ++i){
        res.push_back(other.strokes_[i]);
    }
    return Peal(res.data(), static_cast<int>(res.size()));
}

Peal& Peal::operator+=(const Peal& other) {
    *this = *this + other;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_){
        return false;
    }
    for (int i = 0; i < count_; ++i){
        if (strokes_[i].bell != other.strokes_[i].bell || strokes_[i].beats != other.strokes_[i].beats){
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return !(*this == other);
}

Peal::operator int() const {
    if (count_ == 0) {
        return 0;
    }
    int sum = 0;
    for (int i = 0; i < count_; ++i){
        sum += strokes_[i].beats;
    }
    return sum;
}

}  // namespace rybinsk
