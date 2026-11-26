#include "rybinsk/peal.hpp"

namespace rybinsk {
namespace {

[[noreturn]] void notReady() {
    // Не PealError: иначе тест на ошибку длины станет зелёным раньше решения.
    throw std::logic_error("ещё не написано");
}

}  // namespace

Peal::Peal() {}

Peal::Peal(int count) {

    if (count < 0){
        throw PealError("count < 0");
    }
    if (count > 0){
        strokes_ = new Stroke[count]{};
    }
    count_ = count;

}

Peal::Peal(const Stroke* strokes, int count) {

    if (count < 0){
        throw PealError("count < 0");
    }
    if (count > 0 and strokes == nullptr){
        throw PealError("count_ > 0 and strokes == nullptr)");
    }
    count_ = count;
    if(count_ >  0){
     
        strokes_ = new Stroke[count_]{};
        for (int i=0; i < count; i++){
            strokes_[i] = strokes[i];
        }
    }    
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;

    if(count_ >  0){
        strokes_ = new Stroke[count_]{};
        for (int i=0; i < count_; i++){
            strokes_[i] = other.strokes_[i];
        }
    }    

}

// noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
// Пока источник не опустошается, тесты на перемещение красные.
Peal::Peal(Peal&& other) noexcept {
    strokes_ = other.strokes_;
    count_ = other.count_;
    other.strokes_ = nullptr;
    other.count_ = 0;

}

Peal::~Peal() {
    delete[] strokes_;
    strokes_ = nullptr;
    // Освободите strokes_. delete[] nullptr вызывать можно.
}

Peal& Peal::operator=(const Peal& other) {
    if (this == &other){
        return *this;
    }
    delete[] strokes_;
    strokes_ = nullptr;
    count_ = other.count_;
    if (count_ > 0){
        strokes_ = new Stroke[count_]{};
        for (int i=0; i < count_; i++){
            strokes_[i] = other.strokes_[i];
        }
    }
    return *this;

}

// Заглушка ничего не делает. Для присваивания себе это случайно похоже
// на правильный ответ, чужое перемещение она не выполняет.
Peal& Peal::operator=(Peal&& other) noexcept {
    if (this == &other){
        return *this;
    }
    delete[] strokes_;
    strokes_ = other.strokes_;
    other.strokes_ = nullptr;
    count_ = other.count_;
    other.count_ = 0;

    return *this;
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int index) {
    if (index < 0 or index >= count_){
        throw PealError("index out of range");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if (index < 0 or index >= count_){
        throw PealError("index out of range");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    if (count_ == 0) return other;
    if (other.count_ == 0) return *this;  
    int cmn = 0;
    Peal result(count_ + other.count_);
    if (count_ >= other.count_){
        cmn = other.count_;
    
    }
    else{
        cmn = count_;
    }
    int pos = 0;
    for (int i = 0; i < cmn; i++)
    {
        if(other.strokes_[i].bell == strokes_[i].bell){
            if (other.strokes_[i].beats >= strokes_[i].beats){
            result[pos] = other.strokes_[i];
            }
            else{
                result[pos] = strokes_[i];
            }
            pos +=1;
        }
        else{
            if(other.strokes_[i].bell > strokes_[i].bell){
                result[pos+1] = other.strokes_[i];
                result[pos] = strokes_[i];

            }
            else{
                result[pos] = other.strokes_[i];
                result[pos+1] = strokes_[i];
            }
            pos+=2;
        }
    }
    if (other.count_ > count_)
        {
            for (int i = cmn; i < other.count_; i++)
            {
                result[pos] = other.strokes_[i];
                pos+=1;
            }

        }
        else
        {
            for (int i = cmn; i < count_; i++)
            {
                result[pos] = strokes_[i];
                pos+=1;
            }   
        }
    result.count_ = pos;
    return result;

}

Peal& Peal::operator+=(const Peal& other) {
    return *this = *this + other;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_){
        return false;
    }
    for (int i=0; i < count_; i++){
            if (strokes_[i].bell != other.strokes_[i].bell or strokes_[i].beats != other.strokes_[i].beats){
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
      
    int c = 0;
    for (int i=0; i < count_; i++){
        c+=strokes_[i].beats;
    }
    return c;
}

}  // namespace rybinsk
