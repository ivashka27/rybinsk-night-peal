#include "rybinsk/peal.hpp"

#include <functional>

namespace rybinsk {
namespace {

[[noreturn]] void notReady() {
    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
    throw std::logic_error("ещё не написано");
}

}  // namespace

Peal::Peal() {}

Peal::Peal(int count) {
   if (count<0) {
       throw PealError("");
   }
    if (count==0) {
        strokes_ = nullptr;
        count_ = 0;

    }
    if (count>0) {
        strokes_ = new Stroke[count];
        for (int i = 0; i<count; ++i) {
            strokes_[i]= Stroke{0,0};
        }
        count_ = count;
    }

}

Peal::Peal(const Stroke* strokes, int count) {
    if (count < 0) {
        throw PealError("");
    }
    if (count>0 and strokes == nullptr) {
        throw PealError("");
    }
    if (count==0) {
        strokes_ = nullptr;
    }
    if (count>0) {
        strokes_ = new Stroke[count];
        for (int i = 0; i<count; ++i) {
            strokes_[i]= strokes[i];
        }
        count_ = count;
    }
}

Peal::Peal(const Peal& original_baXX) {
    int count2 = original_baXX.count_;
    strokes_ = new Stroke[count2];
    if (count2==0) {
        strokes_=nullptr;
        count_=0;
    }
    for (int i = 0; i < count2; ++i) {
        strokes_[i] = original_baXX[i];
    }
    count_ = count2;
}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept {
    strokes_ = other.strokes_;
    count_ = other.size();
    other.strokes_ = nullptr;
    other.count_ = 0;
}

Peal::~Peal() {
    if (count_>0) {
        delete[] strokes_;
    }
    strokes_=nullptr;
    count_ = 0;
}

Peal& Peal::operator=(const Peal& other) {
    if (&strokes_!=&other.strokes_) {
        Stroke* temp = nullptr;
        if (other.size()>0) {
            temp = new Stroke[other.size()];
            for (int i = 0; i<other.size(); ++i) {
                temp[i] = other[i];
            }
            count_ = other.count_;
        }
        else {
            count_ = 0;
        }
        delete [] strokes_;
        strokes_ = temp;
    }
    return *this;
}

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
Peal& Peal::operator=(Peal&& other) noexcept {
    if (&strokes_!=&other.strokes_) {
        delete[] strokes_;
        strokes_ = other.strokes_;
        other.strokes_ = nullptr;
        count_ = other.count_;
        other.count_ = 0;
    }
    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int ind) {
    if (0<=ind and ind < this->size()) {
        return strokes_[ind];
    }
    else {
        throw PealError("");
    }
}

const Stroke& Peal::operator[](int ind) const {
    if (0<=ind and ind < this->size()) {
        return strokes_[ind];
    }
    else {
        throw PealError("");
    }
}

Peal Peal::operator+(const Peal& other) const {
    Peal result;
    int misize = std::min(this->size(),other.size());
    int maxsize = std::max(this->size(),other.size());
    int unique_strokes = 0;

    for (int i = 0; i<misize; ++i) {
        if (this->strokes_[i].bell!=other.strokes_[i].bell) {
            ++unique_strokes;
            ++unique_strokes;
        }
        else {
            ++unique_strokes;
        }
    }
    unique_strokes+=(maxsize-misize);
    if (unique_strokes == 0) {
        return result;
    }
    int temp_new_stroke = 0;
    result.strokes_ = new Stroke[unique_strokes];
    result.count_ = unique_strokes;
    for (int i = 0; i < maxsize; ++i) {
        if (i<this->size() and i<other.size()) {
            int kol_num1 = this->strokes_[i].bell;
            int doli1 = this->strokes_[i].beats;
            int kol_num2 = other.strokes_[i].bell;
            int doli2 = other.strokes_[i].beats;
            if (kol_num1!=kol_num2) {
                if (kol_num1<kol_num2) {
                    result.strokes_[temp_new_stroke] = this->strokes_[i];
                    ++temp_new_stroke;
                    result.strokes_[temp_new_stroke] = other.strokes_[i];
                    ++temp_new_stroke;
                }
                else {
                    result.strokes_[temp_new_stroke] = other.strokes_[i];
                    ++temp_new_stroke;
                    result.strokes_[temp_new_stroke] = this->strokes_[i];
                    ++temp_new_stroke;
                }
            }
            else {
                if (doli1!=doli2) {
                    if (doli1 < doli2) {
                        result.strokes_[temp_new_stroke] = other.strokes_[i];
                        ++temp_new_stroke;
                    }
                    else {
                        result.strokes_[temp_new_stroke] = this->strokes_[i];
                        ++temp_new_stroke;
                    }
                }
                else {
                    result.strokes_[temp_new_stroke] = this->strokes_[i];
                    ++temp_new_stroke;
                }
            }
        }
        else if (i<this->size() and i>=other.size()){
            result.strokes_[temp_new_stroke]=this->strokes_[i];
            ++temp_new_stroke;
        }
        else if (i>=this->size() and i<other.size()){
            result.strokes_[temp_new_stroke] = other.strokes_[i];
            ++temp_new_stroke;
        }
    }
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    return (*this = *this + other);
}

bool Peal::operator==(const Peal& other) const {
    if (this->size() != other.size()) return false;
    for (int i=0; i < this->size(); ++i) {
        if (this->strokes_[i].beats!=other.strokes_[i].beats or
            this -> strokes_[i].bell != other.strokes_[i].bell) {
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    return (*this == other ? false : true);
}

Peal::operator int() const {
    int result = 0;
    if (count_ == 0) {
        return 0;
    }
    for (int i = 0; i< count_; ++i) {
        result+=strokes_[i].beats;
    }
    return result;
}

}  // namespace rybinsk
