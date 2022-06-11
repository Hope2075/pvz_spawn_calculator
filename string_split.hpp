#ifndef STRING_SPLIT_HPP_
#define STRING_SPLIT_HPP_

#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>

static const std::vector<std::string> scene_desc[] = {
    {"DAY"     ,"DE" ,"D" ,"白天"  },
    {"NIGHT"   ,"NE" ,"N" ,"夜晚"  ,"黑夜"},
    {"POOL"    ,"PE" ,"P" ,"泳池"  },
    {"FOG"     ,"FE" ,"F" ,"浓雾"  ,"迷雾"},
    {"ROOF"    ,"RE" ,"R" ,"屋顶"  },
    {"MOON"    ,"ME" ,"M" ,"月夜"  },
    {"MUSHROOM","MGE","MG","蘑菇园"},
    {"AQUARIOM","AQE","AQ","水族馆"}
};

static const std::vector<std::string> zombie_desc[20] = {
    {"普僵","REGULAR"  ,"普","普通"},
    {"路障","CONE"     ,"障"},
    {"撑杆","POLE"     ,"杆"},
    {"铁桶","BUCKET"   ,"桶"},
    {"读报","NEWSPAPER","报"},
    {"铁门","DOOR"     ,"门"},
    {"橄榄","FOOTBALL" ,"橄","大爷"},
    {"舞王","DANCE"    ,"舞"},
    {"潜水","SNORKEL"  ,"潜"},
    {"冰车","ZOMBONI"  ,"车"},
    {"海豚","SOLPHIN"  ,"豚"},
    {"小丑","JACK"     ,"丑"},
    {"气球","BALLOON"  ,"气"},
    {"矿工","DIGGER"   ,"矿"},
    {"跳跳","POGO"     ,"跳"},
    {"蹦极","BUNGEE"   ,"偷","小偷"},
    {"扶梯","LADDER"   ,"梯","梯子"},
    {"投篮","CATAPULT" ,"篮"},
    {"白眼","GARG"     ,"白"},
    {"红眼","GIGA"     ,"红"}
};

inline int get_scene(std::string s) {
    // 这个似乎要看 locale
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    for(int i = 0; i < 8; ++i) {
        for(auto j = scene_desc[i].begin(); j != scene_desc[i].end(); ++j) {
            if(s == *j) {
                return i;
            }
        }
    }
    throw std::invalid_argument("");
}

static const std::vector<std::string> splits = {
    "",
    " ",
    ",",
    "，",
    ".",
    "\t"
};

inline int get_zombies(std::string s) {
    //no split
    int res = 0;
    int len = s.length();
    bool err = false;
    for(int i = 0; i < len; i += 2) {
        if( s[i] == ' ' || s[i] == ',' ) {
            --i;
            continue;
        }
        std::string sub = s.substr(i,2);
        bool f = false;
        for(int j = 0; j < 20; ++j) {
            if(sub == zombie_desc[j][2]) {
                res |= (1<<j);
                f = true;
                break;
            }
        }
        if(!f) {
            err = true;
            break;
        }
    }
    if(!err) {
        return res;
    }
    err = false;
    res = 0;
    while(s.length()) {
        auto t0 = s.length();
        auto idx = splits.begin();
        for(auto i = ++splits.begin(); i != splits.end(); ++i) {
            auto t = s.find(*i,0);
            if(t != std::string::npos && t < t0) { 
                t0 = t;
                idx = i;
            }
        }
        std::string s1 = s.substr(0,t0);
        s = s.substr(t0+idx->length());
        if( s1.length() == 0 )
            continue;
        bool f = false;
        for(int i = 0; i < 20; ++i) {
            for(auto j = zombie_desc[i].begin(); j != zombie_desc[i].end(); ++j) {
                if(s1 == *j) {
                    res |= (1<<i);
                    f = true;
                    break;
                }
            }
            if(f)
                break;
        }
        if(!f) {
            err = true;
            break;
        }
    }
    if(err) {
        throw std::invalid_argument("");
    }
    return res;
}

inline int get_single_zombie(std::string s) {
    for(int i = 0; i < 20; ++i){
        for(auto j = zombie_desc[i].begin(); j < zombie_desc[i].end(); ++j) {
            if(s == *j) {
                return i;
            }
        }
    }
    throw std::invalid_argument("");
}


#endif