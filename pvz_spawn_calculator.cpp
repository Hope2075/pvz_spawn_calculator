// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <string>
#include <algorithm>
#include <iomanip>
#include <vector>
#define LENGTH 10
#define BLOCK (1<<LENGTH)
#define MASK (BLOCK-1)
const long long inv=2083697005;
//(int)(inv*101)=1
enum scene{
    DAY      ,NIGHT    ,POOL     ,FOG      ,
    ROOF     ,MOON     ,MUSHROOM ,AQUARIOM
};
enum zombie{
    REGULAR  ,CONE     ,POLE     ,BUCKET   ,
    NEWS     ,DOOR     ,FOOTBALL ,DANCE    ,
    SNORKEL  ,ZOMBONI  ,DOLPHIN  ,JACK     ,
    BALLOON  ,DIGGER   ,POGO     ,BUNGEE   ,
    LADDER   ,CATAPULT ,GARG     ,GIGA
};
const std::vector<std::string> scene_desc[]={
    {"DAY"     ,"DE" ,"D" ,"白天"  },
    {"NIGHT"   ,"NE" ,"N" ,"夜晚"  ,"黑夜"},
    {"POOL"    ,"PE" ,"P" ,"泳池"  },
    {"FOG"     ,"FE" ,"F" ,"浓雾"  ,"迷雾"},
    {"ROOF"    ,"RE" ,"R" ,"屋顶"  },
    {"MOON"    ,"ME" ,"M" ,"月夜"  },
    {"MUSHROOM","MGE","MG","蘑菇园"},
    {"AQUARIOM","AQE","AQ","水族馆"}
};
const std::vector<std::string> zombie_desc[20] = {
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




//FILE* f;

//long long cnt;
//std::mutex mc;
class d_page{
    private:
    class page{
        int id;
        int scene;
        int data[BLOCK][8];
        const int allow_base[8]={
            0x0f0ffaff,0x0f0ff8ff,0x0f0fffff,0x0f0fffff,
            0x0f0fda7f,0x0f0fda7f,0x0f0ffaff,0x0f0ffaff
        };
        const int conv_id[33]={
            31,24, 1, 2, 3,
            4, 5, 6, 7,31,
            31, 8, 9,31,10,
            11,12,13,14,31,
            15,16,17,18,31,25,
            31,31,31,31,31,31,
            19
        };
        class rng{
            uint32_t buf[0x270];
            int idx;
            void rng_twist(){
                int i;
                for (i = 0; i < 0x26F; ++i){
                    uint32_t tmp = (buf[i] & 0x80000000) | (buf[i + 1] & 0x7FFFFFFF);
                    buf[i] = (tmp >> 1) ^ buf[(i + 0x18D) % 0x270];
                    if (tmp & 1){
                        buf[i] ^= 0x9908B0DF;
                    }
                }
            }
            uint32_t rng_gen(){
                uint32_t tmp;
                if (idx == 0)
                    rng_twist();
                tmp = buf[idx];
                tmp ^= (tmp >> 11);
                tmp ^= ((tmp & 0xFF3A58AD) << 7);
                tmp ^= ((tmp & 0xFFFFDF8C) << 15);
                tmp ^= (tmp >> 18);
                idx = (idx + 1) % 0x270;
                return tmp;
            }
            public:
            void rng_init(uint32_t seed){
                int i;
                buf[0] = seed;
                for (i = 1; i < 0x270; ++i){
                    buf[i] =(buf[i - 1] ^ (buf[i - 1] >> 30)) * 0x6C078965 + i;
                }
                idx = 0;
            }
            
            int rnd_choose( int max){
                return (rng_gen() & 0x7FFFFFFF) % max;
            }
        };
        public:
        void init(int sc,int idx){
            rng g;
            int d1[10];
            int allow;
            scene=sc;
            id=idx;
            allow=allow_base[scene];
            uint32_t cur_seed=id*BLOCK;
            for(int i=0;i<BLOCK;i++){
                uint32_t rng_seed=cur_seed*101;
                if(rng_seed==0)rng_seed=0x1105;
                g.rng_init(rng_seed);
                cur_seed+=1;
                int has=1;
                d1[0]=1;
                if(g.rnd_choose(5)==0){
                    d1[0]|=(1<<NEWS);
                    has|=(1<<NEWS);
                }else{
                    d1[0]|=(1<<CONE);
                    has|=(1<<CONE);
                }
                int j=1;
                while(j<10){
                    int type = g.rnd_choose(33);
                    int nid=conv_id[type];
                    //if(!(allow&(1<<nid)))continue;
                    if(has&(1<<nid))continue;
                    if(allow&(1<<nid)){
                        has|=(1<<nid);
                        d1[j]=d1[j-1];
                        if(nid!=24 && nid!=25)d1[j]|=(1<<nid);
                        j++;
                    }
                }
                for(j=3;j<7;j++){
                    if(d1[j]&(1<<GIGA))d1[j]=d1[j+1]^(1<<GIGA);
                }
                if(d1[2]&(1<<ZOMBONI)){
                    if(d1[2]&(1<<GARG)){
                        d1[2]=d1[4]^(1<<ZOMBONI)^(1<<GARG);
                    }else{
                        d1[2]=d1[3]^(1<<ZOMBONI);
                    }
                }else{
                    if(d1[2]&(1<<GARG)){
                        d1[2]=d1[3]^(1<<GARG);
                    }
                }
                for(j=0;j<8;j++){
                    data[i][j]=d1[j+2];
                }
            }
        }
        int get(uint32_t pos,int level){
            return data[pos][level];
        }
    };
    page p0,p1;
    int cur;
    int scene;
    public:
    d_page(){}
    d_page(int sc){
        scene=sc;
        p0.init(sc,0);
        p1.init(sc,1);
    }
    int get(uint32_t pos,int level){
        if(level==0)return 11;
        level--;
        if(level>=8)level=7;
        uint32_t t=pos>>LENGTH;
        if(t==uint32_t(cur+2)){
            if(cur&1){
                p1.init(scene,cur+2);
            }else{
                p0.init(scene,cur+2);
            }
            cur++;
        }
        if(t==uint32_t(cur-1)){
            if(cur&1){
                p1.init(scene,cur-1);
            }else{
                p0.init(scene,cur-1);
            }
            cur--;
        }
        if(t!=uint32_t(cur) && t!=uint32_t(cur+1)){
            if(t&1){
                p1.init(scene,t);
                p0.init(scene,t+1);
            }else{
                p1.init(scene,t+1);
                p0.init(scene,t);
            }
            cur=t;
        }
        if(t&1){
            return p1.get(pos&MASK,level);
        }else{
            return p0.get(pos&MASK,level);
        }
    }
};
void disp(int x){
    bool f=0;
    for(int i=0;i<20;i++){
        if(x&(1<<i)){
            if(f){
                std::cout<<", ";
            }
            f=1;
            std::cout<<zombie_desc[i][0];
        }
    }
}
void calc_single(uint32_t seed,int uid,int mode,int scene,int begin,int end){
    d_page p1(scene);
    uint32_t l=(seed+uid+mode)*inv;
    //p1.reset(scene,l);
    for(int i=begin;i<end;i++){
        int t=p1.get(l+i,i);
        std::cout<<std::setw(4)<<i*2+1<<" - "<<std::setw(4)<<i*2+2<<" Flag: ";
        disp(t);
        std::cout<<std::endl;
    }
}

class worker_base{
    private:
    uint32_t cur;
    bool allow_negative;
    long long rest;
    uint32_t block;
    long long nxt_disp;

    protected:
    
    uint32_t offset;
    int scene;
    class task{
        public:
        uint32_t begin;
        uint32_t end;
        task(uint32_t b,uint32_t e){
            begin=b;
            end=e;
        }
    };
    std::mutex mtx;
    task get(){
        if(rest==0)return task(0,0);
        uint32_t begin=cur;
        uint32_t end=((cur>>LENGTH)+64)<<LENGTH;
        cur=end;
        if(!allow_negative){
            uint32_t s1=end*101-offset;
            if(s1>=0x80000000){
                uint32_t delta=s1-0x80000000;
                end-=delta/101;
                cur+=(uint32_t)(-s1+100)/101;
            }
        }
        uint32_t length=end-begin;
        if(length>=rest){
            end=begin+rest;
            length=rest;
        }
        rest-=length;
        if(rest<=nxt_disp){
            putchar('.');
            nxt_disp-=block;
        }
        return task(begin,end);
    }
    virtual void work()=0;

    public:
    void set(uint32_t begin,uint32_t uid,uint32_t mode,int sc,long long amt,bool neg){
        offset=uid+mode;
        cur=(begin+offset)*inv;
        rest=amt;
        allow_negative=neg;
        block=amt/100;
        nxt_disp=amt-block;
        scene=sc;
    }
    long long times;
    void works(int cnt){
        long long t1=clock();
        for(int i=0;i<100;i++){
            putchar('=');
        }
        putchar('\n');
        std::thread ls[cnt];
        for(int i=0;i<cnt;i++){
            ls[i]=std::thread([=](){
                work();
            });
        }
        for(int i=0;i<cnt;i++){
            ls[i].join();
        }
        putchar('\n');
        long long t2=clock();
        times=t2-t1;
    }
};

class worker_satisfy :public worker_base{
    public:
    uint32_t result;
    bool found;
    protected:
    virtual bool check(uint32_t,d_page&)=0;
    
    private:
    void work(){
        d_page p(scene);
        bool r1=0;
        uint32_t r2;
        while(1){
            mtx.lock();
            task t=get();
            mtx.unlock();
            if(t.begin==t.end)break;
            uint32_t cur=t.begin;
            while(cur!=t.end){
                if(check(cur,p)){
                    r1=1;
                    r2=cur;
                    break;
                }
                cur++;
            }
            mtx.lock();
            if(r1){
                if(!found){
                    found=1;
                    result=r2*101-offset;
                }
            }
            if(found){
                mtx.unlock();
                break;
            }
            mtx.unlock();

        }
    }
};
class worker_type :public worker_satisfy{
    public:
    int begin,end,include[1<<10],exclude[1<<10];
    private:
    bool check(uint32_t i_seed,d_page& p){
        for(int i=begin;i<end;i++){
            int t;
            t=p.get(i_seed+i,i);
            if(t&exclude[i])return 0;
            if((t&include[i])!=include[i])return 0;
        }
        return 1;
    }
};
class worker_limit :public worker_satisfy{
    public:
    int begin,end,type,limit;
    private:
    bool check(uint32_t i_seed,d_page& p){
        int cnt=0;
        for(int i=begin;i<end;i++){
            int t;
            t=p.get(i_seed+i,i);
            if(t&(1<<type))cnt++;
            if(cnt>limit)return 0;
        }
        return 1;
    }
};

int get_scene(std::string s){
    std::transform(s.begin(),s.end(),s.begin(),::toupper);
    for(int i=0;i<8;i++){
        for(auto j=scene_desc[i].begin();j!=scene_desc[i].end();j++){
            if(s==*j){
                return i;
            }
        }
    }
    throw std::invalid_argument("");
}
std::vector<std::string> splits={
    "",
    " ",
    ",",
    "，",
    ".",
    "\t"
};
int get_zombies(std::string s){
    //no split
    int res=0;
    int len=s.length();
    bool err=0;
    for(int i=0;i<len;i+=2){
        if(s[i]==' '||s[i]==','){
            i--;
            continue;
        }
        std::string sub=s.substr(i,2);
        bool f=0;
        for(int j=0;j<20;j++){
            if(sub==zombie_desc[j][2]){
                res|=(1<<j);
                f=1;
                break;
            }
        }
        if(!f){
            err=1;
            break;
        }
    }
    if(!err){
        return res;
    }
    err=0;
    res=0;
    while(s.length()){
        auto t0=s.length();
        auto idx=splits.begin();
        for(auto i=++splits.begin();i!=splits.end();i++){
            auto t=s.find(*i,0);
            if(t!=std::string::npos && t<t0){
                
                t0=t;
                idx=i;
            }
        }
        std::string s1=s.substr(0,t0);
        s=s.substr(t0+idx->length());
        if(s1.length()==0)continue;
        bool f=0;
        for(int i=0;i<20;i++){
            for(auto j=zombie_desc[i].begin();j!=zombie_desc[i].end();j++){
                if(s1==*j){
                    res|=(1<<i);
                    f=1;
                    break;
                }
            }
            if(f)break;
        }
        if(!f){
            err=1;
            break;
        }
    }
    if(err){
        throw std::invalid_argument("");
    }
    return res;
}
int get_single_zombie(std::string s){
    for(int i=0;i<20;i++){
        for(auto j=zombie_desc[i].begin();j<zombie_desc[i].end();j++){
            if(s==*j){
                return i;
            }
        }
    }
    throw std::invalid_argument("");
}
int main(){
    while(1){
        std::cout<<"1. 根据种子计算出怪"<<std::endl;
        std::cout<<"2. 按每关的出怪求出种子"<<std::endl;
        std::cout<<"3. 限制单个怪的次数，求出种子"<<std::endl;
        std::cout<<"选择模式，或按回车退出: ";
        std::string mode;
        std::getline(std::cin,mode);
        if(mode=="1"){
            uint32_t seed;
            int mode,uid,scene,begin,end;
            std::string input;

            std::cout<<"种子(十六进制): ";
            std::getline(std::cin,input);
            try{
                long long s0=std::stoll(input,0,16);
                if(s0<0 || s0>=(1ll<<32))throw std::out_of_range("");
                seed=s0;
            }catch(std::invalid_argument const& e){
                std::cout<<"种子格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"种子数值非法！"<<std::endl;
                continue;
            }

            std::cout<<"场景(默认为泳池): ";
            std::getline(std::cin,input);
            if(input==""){
                scene=POOL;
            }else{
                try{
                    scene=get_scene(input);
                }catch(std::invalid_argument const& e){
                    std::cout<<"场景格式错误！"<<std::endl;
                    continue;
                }
            }
            

            std::cout<<"用户编号(默认为1): ";
            std::getline(std::cin,input);
            if(input==""){
                uid=1;
            }else{
                try{
                    uid=std::stoul(input,0,10);
                }catch(std::invalid_argument const& e){
                    std::cout<<"用户编号格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"用户编号数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"存档编号(默认为13): ";
            std::getline(std::cin,input);
            if(input==""){
                mode=13;
            }else{
                try{
                    mode=std::stoul(input,0,10);
                }catch(std::invalid_argument const& e){
                    std::cout<<"存档编号格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"存档编号数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"起始旗帜数(奇数): ";
            std::getline(std::cin,input);
            try{
                begin=std::stol(input,0,10);
                if(begin<0||begin%2==0)throw std::out_of_range("");
                begin/=2;
            }catch(std::invalid_argument const& e){
                std::cout<<"起始旗帜数格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"起始旗帜数数值非法！"<<std::endl;
                continue;
            }

            std::cout<<"终止旗帜数(偶数)，直接输入回车则计算当前关: ";
            std::getline(std::cin,input);
            try{
                if(input==""){
                    end=begin+1;
                }else{
                    end=std::stol(input,0,10);
                    if(end<0||end%2==1)throw std::out_of_range("");
                    end/=2;
                    if(end<=begin)throw std::out_of_range("");
                }
            }catch(std::invalid_argument const& e){
                std::cout<<"终止旗帜数格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"终止旗帜数数值非法！"<<std::endl;
                continue;
            }
            calc_single(seed,uid,mode,scene,begin,end);
            //calc_single(0x09a6313b,1,13,POOL,0,40);
        }else if(mode=="2"){
            uint32_t seed;
            int mode,uid,scene,begin,end;
            long long maxn;
            std::string input;
            worker_type wk;

            std::cout<<"起始种子(十六进制)(默认为0): ";
            std::getline(std::cin,input);
            if(input==""){
                seed=0;
            }else{
                try{
                    long long s0=std::stoll(input,0,16);
                    if(s0<0 || s0>=(1ll<<32))throw std::out_of_range("");
                    seed=s0;
                }catch(std::invalid_argument const& e){
                    std::cout<<"种子格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"种子数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"最大范围(默认计算所有种子): ";
            std::getline(std::cin,input);
            try{
                if(input==""){
                    maxn=1ll<<31;
                }else{
                    maxn=std::stol(input,0,10);
                    if(maxn<=0||maxn>(1ll<<31))throw std::out_of_range("");
                }
            }catch(std::invalid_argument const& e){
                std::cout<<"最大范围格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"最大范围数值非法！"<<std::endl;
                continue;
            }

            std::cout<<"场景(默认为泳池): ";
            std::getline(std::cin,input);
            if(input==""){
                scene=POOL;
            }else{
                try{
                    scene=get_scene(input);
                }catch(std::invalid_argument const& e){
                    std::cout<<"场景格式错误！"<<std::endl;
                    continue;
                }
            }
            

            std::cout<<"用户编号(默认为1): ";
            std::getline(std::cin,input);
            if(input==""){
                uid=1;
            }else{
                try{
                    uid=std::stoul(input,0,10);
                }catch(std::invalid_argument const& e){
                    std::cout<<"用户编号格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"用户编号数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"存档编号(默认为13): ";
            std::getline(std::cin,input);
            if(input==""){
                mode=13;
            }else{
                try{
                    mode=std::stoul(input,0,10);
                }catch(std::invalid_argument const& e){
                    std::cout<<"存档编号格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"存档编号数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"起始旗帜数(奇数): ";
            std::getline(std::cin,input);
            try{
                begin=std::stol(input,0,10);
                if(begin<0||begin%2==0)throw std::out_of_range("");
                begin/=2;
            }catch(std::invalid_argument const& e){
                std::cout<<"起始旗帜数格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"起始旗帜数数值非法！"<<std::endl;
                continue;
            }

            std::cout<<"终止旗帜数(偶数)，直接输入回车则计算当前关: ";
            std::getline(std::cin,input);
            try{
                if(input==""){
                    end=begin+1;
                }else{
                    end=std::stol(input,0,10);
                    if(end<0||end%2==1)throw std::out_of_range("");
                    end/=2;
                    if(end<=begin)throw std::out_of_range("");
                }
            }catch(std::invalid_argument const& e){
                std::cout<<"终止旗帜数格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"终止旗帜数数值非法！"<<std::endl;
                continue;
            }
            wk.begin=begin;
            wk.end=end;
            try{
                int include=1;
                int exclude=0;
                for(int i=begin;i<end;i++){
                    std::cout<<std::setw(4)<<i*2+1<<" -"<<std::setw(4)<<i*2+2<<" flag 的信息"<<std::endl;
                    std::cout<<"包含以下僵尸，输入\"P\"则之后使用上一关的设置：";
                    std::getline(std::cin,input);
                    if(input=="P"){
                        for(int j=i;j<end;j++){
                            wk.include[j]=include;
                            wk.exclude[j]=exclude;
                        }
                        break;
                    }
                    include=get_zombies(input);
                    std::cout<<"不包含以下僵尸，输入回车则不作限制，输入\"A\"则排除其它僵尸：";
                    std::getline(std::cin,input);
                    if(input=="A"){
                        exclude=~include;
                    }else{  
                        exclude=get_zombies(input);
                    }
                    exclude&=0xfffffffe;
                    wk.include[i]=include;
                    wk.exclude[i]=exclude;
                }
            }catch(std::invalid_argument const& e){
                std::cout<<"输入格式错误！"<<std::endl;
                continue;
            }

            wk.set(seed,uid,mode,scene,maxn,0);
            wk.works(8);
            std::cout<<"用时:"<<std::setw(10)<<std::setprecision(3)<<wk.times/1000.0<<"秒"<<std::endl;

            if(wk.found){
                std::cout<<"成功找到种子："<<std::setw(8)<<std::hex<<wk.result<<std::dec<<std::endl;
            }else{
                std::cout<<"未能找到种子"<<std::endl;
            }
        }else if(mode=="3"){
            uint32_t seed;
            int mode,uid,scene,begin,end;
            long long maxn;
            std::string input;
            worker_limit wk;
            int target,limit;

            std::cout<<"起始种子(十六进制)(默认为0): ";
            std::getline(std::cin,input);
            if(input==""){
                seed=0;
            }else{
                try{
                    long long s0=std::stoll(input,0,16);
                    if(s0<0 || s0>=(1ll<<32))throw std::out_of_range("");
                    seed=s0;
                }catch(std::invalid_argument const& e){
                    std::cout<<"种子格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"种子数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"最大范围(默认计算所有种子): ";
            std::getline(std::cin,input);
            try{
                if(input==""){
                    maxn=1ll<<31;
                }else{
                    maxn=std::stol(input,0,10);
                    if(maxn<=0||maxn>(1ll<<31))throw std::out_of_range("");
                }
            }catch(std::invalid_argument const& e){
                std::cout<<"最大范围格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"最大范围数值非法！"<<std::endl;
                continue;
            }

            std::cout<<"场景(默认为泳池): ";
            std::getline(std::cin,input);
            if(input==""){
                scene=POOL;
            }else{
                try{
                    scene=get_scene(input);
                }catch(std::invalid_argument const& e){
                    std::cout<<"场景格式错误！"<<std::endl;
                    continue;
                }
            }
            

            std::cout<<"用户编号(默认为1): ";
            std::getline(std::cin,input);
            if(input==""){
                uid=1;
            }else{
                try{
                    uid=std::stoul(input,0,10);
                }catch(std::invalid_argument const& e){
                    std::cout<<"用户编号格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"用户编号数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"存档编号(默认为13): ";
            std::getline(std::cin,input);
            if(input==""){
                mode=13;
            }else{
                try{
                    mode=std::stoul(input,0,10);
                }catch(std::invalid_argument const& e){
                    std::cout<<"存档编号格式错误！"<<std::endl;
                    continue;
                }catch(std::out_of_range const& e){
                    std::cout<<"存档编号数值非法！"<<std::endl;
                    continue;
                }
            }

            std::cout<<"起始旗帜数(奇数): ";
            std::getline(std::cin,input);
            try{
                begin=std::stol(input,0,10);
                if(begin<0||begin%2==0)throw std::out_of_range("");
                begin/=2;
            }catch(std::invalid_argument const& e){
                std::cout<<"起始旗帜数格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"起始旗帜数数值非法！"<<std::endl;
                continue;
            }

            std::cout<<"终止旗帜数(偶数)，直接输入回车则计算当前关: ";
            std::getline(std::cin,input);
            try{
                if(input==""){
                    end=begin+1;
                }else{
                    end=std::stol(input,0,10);
                    if(end<0||end%2==1)throw std::out_of_range("");
                    end/=2;
                    if(end<=begin)throw std::out_of_range("");
                }
            }catch(std::invalid_argument const& e){
                std::cout<<"终止旗帜数格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"终止旗帜数数值非法！"<<std::endl;
                continue;
            }
            wk.begin=begin;
            wk.end=end;

            std::cout<<"目标僵尸: ";
            std::getline(std::cin,input);
            try{
                target=get_single_zombie(input);
            }catch(std::invalid_argument const& e){
                std::cout<<"目标僵尸数格式错误！"<<std::endl;
                continue;
            }

            std::cout<<"最大次数(默认值为0): ";
            std::getline(std::cin,input);
            try{
                if(input==""){
                    limit=0;
                }else{
                    limit=std::stol(input,0,10);
                    if(limit<0||begin+limit>end)throw std::out_of_range("");
                }
            }catch(std::invalid_argument const& e){
                std::cout<<"最大次数格式错误！"<<std::endl;
                continue;
            }catch(std::out_of_range const& e){
                std::cout<<"最大次数数值非法！"<<std::endl;
                continue;
            }
            wk.times=limit;
            wk.type=target;
            //std::cout<<"DEBUG:"<<limit<<" "<<target<<std::endl;
            wk.set(seed,uid,mode,scene,maxn,0);
            wk.works(8);
            std::cout<<"用时:"<<std::setw(10)<<std::setiosflags(std::ios::fixed)<<std::setprecision(3)<<wk.times/1000.0<<"秒"<<std::endl;
            if(wk.found){
                std::cout<<"成功找到种子："<<std::setw(8)<<std::hex<<wk.result<<std::dec<<std::endl;
            }else{
                std::cout<<"未能找到种子"<<std::endl;
            }
        }else if(mode==""){
            break;
        }else{
            std::cout<<"模式编号错误"<<std::endl;
        }
    }
}
