#include "rybinsk/voice.hpp"

namespace rybinsk {
namespace {

bool strIsOk(Stroke str){
    if(str.bell==0){
        return str.beats==1;
    }
    if(str.bell>=1 and str.bell<=8){
        return str.beats>=1;
    }
    return false;
}

}  // namespace

Voice voiceOf(const Peal& sheet){
    Voice res;
    res.accepted=true;
    res.peal=Peal();

    if(sheet.size()==0){
        return res;
    }

    for(int idx=0;idx<sheet.size();idx++){
        if(not strIsOk(sheet[idx])){
            res.accepted=false;
            res.peal=Peal();
            return res;
        }
    }

    Stroke* clean_str=new Stroke[sheet.size()];
    int clean_cnt=0;
    bool has_last_bell=false;
    int last_bell=0;

    for(int idx=0;idx<sheet.size();idx++){
        Stroke cur=sheet[idx];
        if(cur.bell==0){
            clean_str[clean_cnt]=cur;
            clean_cnt++;
            has_last_bell=false;
            continue;
        }
        if(has_last_bell and cur.bell==last_bell){
            continue;
        }
        clean_str[clean_cnt]=cur;
        clean_cnt++;
        has_last_bell=true;
        last_bell=cur.bell;
    }

    res.accepted=true;
    res.peal=Peal(clean_str,clean_cnt);
    delete[] clean_str;
    return res;
}

}  // namespace rybinsk
