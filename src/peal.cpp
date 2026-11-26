#include "rybinsk/peal.hpp"

namespace rybinsk {
namespace {

Stroke* allocZeroStr(int cnt){
    Stroke* str=new Stroke[cnt];
    for(int idx=0;idx<cnt;idx++){
        str[idx].bell=0;
        str[idx].beats=0;
    }
    return str;
}

Stroke* allocCpyStr(const Stroke* src,int cnt){
    Stroke* str=new Stroke[cnt];
    for(int idx=0;idx<cnt;idx++){
        str[idx]=src[idx];
    }
    return str;
}

void appendStr(Stroke* buf,int& wr_cnt,Stroke str_to_add){
    buf[wr_cnt]=str_to_add;
    wr_cnt++;
}

void resolvePair(Stroke left_str,Stroke right_str,Stroke* buf,int& wr_cnt){
    if(left_str.bell==right_str.bell){
        if(right_str.beats>left_str.beats){
            appendStr(buf,wr_cnt,right_str);
        }else{
            appendStr(buf,wr_cnt,left_str);
        }
    }else if(left_str.bell<right_str.bell){
        appendStr(buf,wr_cnt,left_str);
        appendStr(buf,wr_cnt,right_str);
    }else{
        appendStr(buf,wr_cnt,right_str);
        appendStr(buf,wr_cnt,left_str);
    }
}

}  // namespace

Peal::Peal(){
    strokes_=nullptr;
    count_=0;
}

Peal::Peal(int cnt){
    if(cnt<0){
        throw PealError("отрицательная длина партии");
    }
    if(cnt==0){
        strokes_=nullptr;
        count_=0;
        return;
    }
    strokes_=allocZeroStr(cnt);
    count_=cnt;
}

Peal::Peal(const Stroke* src,int cnt){
    if(cnt<0){
        throw PealError("отрицательная длина партии");
    }
    if(cnt>0 and src==nullptr){
        throw PealError("пустой указатель при ненулевой длине");
    }
    if(cnt==0){
        strokes_=nullptr;
        count_=0;
        return;
    }
    strokes_=allocCpyStr(src,cnt);
    count_=cnt;
}

Peal::Peal(const Peal& oth){
    if(oth.count_==0){
        strokes_=nullptr;
        count_=0;
        return;
    }
    strokes_=allocCpyStr(oth.strokes_,oth.count_);
    count_=oth.count_;
}

Peal::Peal(Peal&& oth) noexcept{
    strokes_=oth.strokes_;
    count_=oth.count_;
    oth.strokes_=nullptr;
    oth.count_=0;
}

Peal::~Peal(){
    delete[] strokes_;
    strokes_=nullptr;
    count_=0;
}

Peal& Peal::operator=(const Peal& oth){
    if(this==&oth){
        return *this;
    }
    Stroke* cpy=nullptr;
    if(oth.count_>0){
        cpy=allocCpyStr(oth.strokes_,oth.count_);
    }
    delete[] strokes_;
    strokes_=cpy;
    count_=oth.count_;
    return *this;
}

Peal& Peal::operator=(Peal&& oth) noexcept{
    if(this==&oth){
        return *this;
    }
    delete[] strokes_;
    strokes_=oth.strokes_;
    count_=oth.count_;
    oth.strokes_=nullptr;
    oth.count_=0;
    return *this;
}

int Peal::size() const{
    return count_;
}

Stroke& Peal::operator[](int idx){
    if(idx<0 or idx>=count_){
        throw PealError("индекс удара вне партии");
    }
    return strokes_[idx];
}

const Stroke& Peal::operator[](int idx) const{
    if(idx<0 or idx>=count_){
        throw PealError("индекс удара вне партии");
    }
    return strokes_[idx];
}

Peal Peal::operator+(const Peal& oth) const{
    if(count_==0){
        return Peal(oth);
    }
    if(oth.count_==0){
        return Peal(*this);
    }

    int pair_cnt=count_;
    if(oth.count_<pair_cnt){
        pair_cnt=oth.count_;
    }

    int max_len=count_+oth.count_;
    Stroke* res_buf=new Stroke[max_len];
    int wr_cnt=0;

    for(int idx=0;idx<pair_cnt;idx++){
        resolvePair(strokes_[idx],oth.strokes_[idx],res_buf,wr_cnt);
    }

    if(count_>oth.count_){
        for(int idx=pair_cnt;idx<count_;idx++){
            appendStr(res_buf,wr_cnt,strokes_[idx]);
        }
    }else{
        for(int idx=pair_cnt;idx<oth.count_;idx++){
            appendStr(res_buf,wr_cnt,oth.strokes_[idx]);
        }
    }

    Peal res(res_buf,wr_cnt);
    delete[] res_buf;
    return res;
}

Peal& Peal::operator+=(const Peal& oth){
    Peal comb=*this+oth;
    *this=comb;
    return *this;
}

bool Peal::operator==(const Peal& oth) const{
    if(count_!=oth.count_){
        return false;
    }
    for(int idx=0;idx<count_;idx++){
        if(strokes_[idx].bell!=oth.strokes_[idx].bell){
            return false;
        }
        if(strokes_[idx].beats!=oth.strokes_[idx].beats){
            return false;
        }
    }
    return true;
}

bool Peal::operator!=(const Peal& oth) const{
    return not(*this==oth);
}

Peal::operator int() const{
    int tot=0;
    for(int idx=0;idx<count_;idx++){
        tot=tot+strokes_[idx].beats;
    }
    return tot;
}

}  // namespace rybinsk
