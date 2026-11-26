#include "rybinsk/peal.hpp"

namespace rybinsk {
namespace {
}  // namespace

Peal::Peal() {
    strokes_ = nullptr;
    count_ = 0;
}

Peal::Peal(int n) {
    if (n<0){
        throw PealError("Количество ударов должно быть не отрицательным");
    }
    if (n>0){
        strokes_ = new Stroke [n]{};
    }
    count_ = n;
}  

Peal::Peal(const Stroke* n, int p) {
    if (p<0){
        throw PealError("Отрицательное количество ударов");
    }
    if (n==nullptr && p>0){
        throw PealError("Указатель пуст");
    }
    if (p>0){
        strokes_ = new Stroke [p];
        for (int i=0;i<p;i++){
            strokes_[i]=n[i];
        }
    }
    count_ = p;
}

Peal::Peal(const Peal& other) {
    count_ = other.count_;
    if (count_==0){
        strokes_ = nullptr;
        return;
    }
    strokes_ = new Stroke[count_];
    for (int i=0;i<count_;i++){
        strokes_[i] = other.strokes_[i];
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
}

Peal& Peal::operator=(const Peal& other) {
    if (this==&other){
        return *this;
    }
    Stroke* copy = nullptr;
    if (other.count_ > 0){
        copy = new Stroke[other.count_];
        for (int i=0; i<other.count_;i++){
            copy[i] = other.strokes_[i];
        }
    }
    delete[] strokes_;
    strokes_ = copy;
    count_ = other.count_;
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
    count_ = other.count_;
    other.strokes_ = nullptr;
    other.count_ = 0;
    return *this;
    
}

int Peal::size() const {
    return count_;
}

Stroke& Peal::operator[](int index) {
    if(index<0 || index >= count_){
        throw PealError("Индексы ударав вне диапозона");
    }
    return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
    if(index<0 || index >= count_){
        throw PealError("Индексы ударав вне диапозона");
    }
    return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
    int c;
    int r;
    if (count_ < other.count_){
        c=count_;
        r=other.count_ - count_;
    }else{
        c=other.count_;
        r=count_ - other.count_;
    }
    for (int i=0;i<c;i++){
        if (strokes_[i].bell == other.strokes_[i].bell){
            r++;
        }else{
            r+=2;
        }
    }
    Peal result(r);
    int out = 0;
    for (int i=0;i<c;i++){
        if (strokes_[i].bell == other.strokes_[i].bell){
            if (strokes_[i].beats >= other.strokes_[i].beats){
                result[out] = strokes_[i];
            }else{
                result[out] = other.strokes_[i];
            }
            out++;
        }else if (strokes_[i].bell < other.strokes_[i].bell){
            result[out] = strokes_[i];
            out++;
            result[out] = other.strokes_[i];
            out++;
        }else{
            result[out] = other.strokes_[i];
            out++;
            result[out] = strokes_[i];
            out++;
        }
    }
    for (int i=c;i<count_;i++){
        result[out] = strokes_[i];
        out++;
    }
    for (int i=c;i<other.count_;i++){
       result[out] = other.strokes_[i];
       out++;
    }
    return result;
}

Peal& Peal::operator+=(const Peal& other) {
    Peal result = *this + other;
    *this = result;
    return *this;
}

bool Peal::operator==(const Peal& other) const {
    if (count_ != other.count_){
        return false;
    }
    for (int i=0;i<count_;i++){
        if (strokes_[i].bell != other.strokes_[i].bell){
            return false;
        }
        if (strokes_[i].beats != other.strokes_[i].beats){
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& other) const {
    if (!(*this == other)){
        return true;
    }else{
        return false;
    }
}

Peal::operator int() const {
    if (count_ == 0) {
        return 0;
    }
    int e=0;
    for (int i=0;i<count_;i++){
        e+= strokes_[i].beats;
    }
    return e;
}

}  // namespace rybinsk
