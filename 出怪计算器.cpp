#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#include<iostream>
#include<thread>
#include<mutex>
#include<ctime>
#include<string>
#include<algorithm>
#include<iomanip>
#include<vector>
int TRDS=1;
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
class io_manager{

    std::vector<std::vector<std::string> > scene_desc;
    std::vector<std::vector<std::string> > zombie_desc;
    std::vector<std::string> splits;
    std::vector<std::string> visiable;
    public:
    io_manager(){
        scene_desc={
            {"DAY"     ,"白天"  ,"DE" ,"D" },
            {"NIGHT"   ,"夜晚"  ,"NE" ,"N" ,"黑夜"},
            {"POOL"    ,"泳池"  ,"PE" ,"P" },
            {"FOG"     ,"浓雾"  ,"FE" ,"F" ,"迷雾"},
            {"ROOF"    ,"屋顶"  ,"RE" ,"R" },
            {"MOON"    ,"月夜"  ,"ME" ,"M" },
            {"MUSHROOM","蘑菇园","MGE","MG"},
            {"AQUARIOM","水族馆","AQE","AQ"}
        };
        zombie_desc={
            {"普僵","普","REGULAR"  ,"普通"},
            {"路障","障","CONE"     },
            {"撑杆","杆","POLE"     },
            {"铁桶","桶","BUCKET"   },
            {"读报","报","NEWSPAPER"},
            {"铁门","门","DOOR"     },
            {"橄榄","橄","FOOTBALL" ,"大爷"},
            {"舞王","舞","DANCE"    },
            {"潜水","潜","SNORKEL"  },
            {"冰车","车","ZOMBONI"  },
            {"海豚","豚","SOLPHIN"  },
            {"小丑","丑","JACK"     },
            {"气球","气","BALLOON"  },
            {"矿工","矿","DIGGER"   },
            {"跳跳","跳","POGO"     },
            {"蹦极","偷","BUNGEE"   ,"小偷"},
            {"扶梯","梯","LADDER"   ,"梯子"},
            {"投篮","篮","CATAPULT" },
            {"白眼","白","GARG"     },
            {"红眼","红","GIGA"     }
        };
        splits={
            "",
            " ",
            ",",
            "，",
            ".",
            "\t",
            ":",
            ";",
            "；"
        };
        visiable={
            "",
            ",",
            "，",
            ".",
            ":",
            ";",
            "；"
        };
    }
    class io_exception{
        public:
        int value;
        io_exception(int v){value=v;}
    };
    void list(){
        for(int i=0;i<20;i++){
            std::cout<<zombie_desc[i][0];
        }
    }
    long long to_number(std::string s,int base=10){
        long long r;
        try{
            r=std::stoll(s,0,base);
        }catch(std::invalid_argument e){
            throw io_exception(3);
        }catch(std::out_of_range e){
            throw io_exception(4);
        }
        return r;
    }
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
    int get_scene(int def=-1){
        std::string s;
        std::getline(std::cin,s);
        if(s=="" && def!=-1)return def;
        for(int i=0;i<8;i++){
            for(auto j=scene_desc[i].begin();j!=scene_desc[i].end();j++){
                if(s==*j){
                    return i;
                }
            }
        }
        throw io_exception(0);
    }
    
    int get_zombies(){
        std::string s;
        std::getline(std::cin,s);
        if(s=="P"||s=="PASS")throw io_exception(1);
        if(s=="A"||s=="ALL"||s=="O"||s=="OTHER")throw io_exception(2);
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
                if(sub==zombie_desc[j][1]){
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
            throw io_exception(0);
        }
        return res;
    }
    int get_single_zombie(){
        std::string s;
        std::getline(std::cin,s);
        for(int i=0;i<20;i++){
            for(auto j=zombie_desc[i].begin();j<zombie_desc[i].end();j++){
                if(s==*j){
                    return i;
                }
            }
        }
        throw io_exception(0);
    }
    long long get_number(long long min=0,long long max=0,long long def=-1){
        std::string input;
        std::getline(std::cin,input);
        if(input==""){
            if(def>=0)return def;
            throw io_exception(0);
        }else{
            long long x=to_number(input);
            if(min!=max && (x<min||x>=max))throw io_exception(0);
            return x;
        }
    }
    long long get_hex(long long min=0,long long max=0,long long def=-1){
        std::string input;
        std::getline(std::cin,input);
        if(input==""){
            if(def>=0)return def;
            throw io_exception(0);
        }else{
            long long x;
            x=to_number(input,16);
            if(min!=max && (x<min||x>=max))throw io_exception(0);
            return x;
        }
    }
    void get_kv(long long value[20]){
        std::string s,s0;
        std::getline(std::cin,s);
        s0=s;
        //no split
        if(s=="P"||s=="PASS")throw io_exception(1);
        for(int i=0;i<20;i++)value[i]=0;
        try{
            int pos=0;
            while(s.length()){
                auto t0=s.length();
                auto idx=visiable.begin();
                bool sp=0;
                for(auto i=++visiable.begin();i!=visiable.end();i++){
                    auto t=s.find(*i,0);
                    if(t!=std::string::npos && t<t0){
                        t0=t;
                        idx=i;
                    }
                }
                std::string s1=s.substr(0,t0);
                s=s.substr(t0+idx->length());
                
                bool f=0;
                while(s1.length()){
                    auto t1=s1.length();
                    auto idx2=splits.begin();
                    for(auto i=++splits.begin();i!=splits.end();i++){
                        auto t=s1.find(*i,0);
                        if(t!=std::string::npos && t<t1){
                            t1=t;
                            idx2=i;
                        }
                    }
                    std::string s2=s1.substr(0,t1);
                    s1=s1.substr(t1+idx2->length());
                    if(s2.length()==0)continue;
                    if(pos>=20)throw io_manager::io_exception(0);
                    f=1;
                    value[pos++]=to_number(s2);
                }
                if(f==0){
                    if(pos==20)throw io_manager::io_exception(0);
                    value[pos++]=0;
                }
            }
        }catch(io_manager::io_exception const& e){
            s=s0;
            int prev=-1;
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
                
                if(prev==-1){
                    for(int i=0;i<20;i++){
                        for(auto j=zombie_desc[i].begin();j!=zombie_desc[i].end();j++){
                            std::string s2=s1.substr(0,j->length());
                            if(s2==*j){
                                try{
                                    value[i]=to_number(s1.substr(j->length()));
                                    f=1;
                                    break;
                                }catch(io_manager::io_exception const& e){}
                            }
                        }
                        if(f)break;
                    }
                }
                if(f){
                    continue;
                }
                f=0;
                for(int i=0;i<20;i++){
                    for(auto j=zombie_desc[i].begin();j!=zombie_desc[i].end();j++){
                        if(s1==*j){
                            if(prev!=-1)throw io_exception(0);
                            prev=i;
                            f=1;
                            break;
                        }
                    }
                    if(f)break;
                }
                if(f){
                    continue;
                }
                if(prev==-1)throw io_exception(0);
                value[prev]=to_number(s1);
                prev=-1;
            }
            return;
        }
    }
    void list_names(){
        std::cout<<"僵尸名称:"<<std::endl;
        std::cout<<std::setw(10)<<"全称"<<std::setw(10)<<"简称"<<std::setw(10)<<"英文"<<std::setw(10)<<"别名"<<std::endl;
        for(int i=0;i<20;i++){
            for(auto j=zombie_desc[i].begin();j!=zombie_desc[i].end();j++){
                std::cout<<std::setw(10)<<*j;
            }
            std::cout<<std::endl;
        }
        std::cout<<"场地名称:"<<std::endl;
        std::cout<<std::setw(10)<<"英文"<<std::setw(10)<<"中文"<<std::setw(10)<<"简称"<<std::setw(10)<<"别名"<<std::endl;
        for(int i=0;i<8;i++){
            for(auto j=scene_desc[i].begin();j!=scene_desc[i].end();j++){
                std::cout<<std::setw(10)<<*j;
            }
            std::cout<<std::endl;
        }
    }
};

class d_page{
    public:
    static const int LENGTH=10;
    static const int BLOCK=(1<<LENGTH);
    static const int MASK=(BLOCK-1);
    
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
        uint32_t buf[0x270];
        int idx;
        uint32_t rng_gen(int max){
            uint32_t tmp;

            tmp=(buf[idx]&0x80000000)|(buf[idx+1]&0x7FFFFFFF);
            buf[idx]=(tmp>>1)^buf[(idx+0x18D)%0x270];
            if(tmp&1){
                buf[idx]^=0x9908B0DF;
            }

            buf[idx+0x18D+1]=(buf[idx+0x18D]^(buf[idx+0x18D]>>30))*0x6C078965+idx+0x18D+1;

            tmp=buf[idx];
            tmp^=(tmp>>11);
            tmp^=((tmp&0xFF3A58AD)<<7);
            tmp^=((tmp&0xFFFFDF8C)<<15);
            tmp^=(tmp>>18);
            idx=(idx+1)%0x270;
            return (tmp&0x7FFFFFFF)%max;
        }
        void rng_init(uint32_t seed){
            buf[0]=seed;
            for(int i=1;i<0x18e;++i){
                buf[i]=(buf[i-1]^(buf[i-1]>>30))*0x6C078965+i;
            }
            idx=0;
        }
        
        
        
        public:
        void init(int sc,int idx){
            int d1[10];
            int allow;
            scene=sc;
            id=idx;
            allow=allow_base[scene];
            uint32_t cur_seed=id*BLOCK;
            for(int i=0;i<BLOCK;i++){
                uint32_t rng_seed=cur_seed*101;
                if(rng_seed==0)rng_seed=0x1105;
                rng_init(rng_seed);
                cur_seed+=1;
                int has=1;
                d1[0]=1;
                if(rng_gen(5)==0){
                    d1[0]|=(1<<NEWS);
                    has|=(1<<NEWS);
                }else{
                    d1[0]|=(1<<CONE);
                    has|=(1<<CONE);
                }
                int j=1;
                while(j<10){
                    int type = rng_gen(33);
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
                for(j=2;j<6;j++){
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
        int t=int(pos>>LENGTH);
        if(t==cur+2){
            //printf("N\n");
            if(cur&1){
                p1.init(scene,cur+2);
            }else{
                p0.init(scene,cur+2);
            }
            cur++;
        }
        if(t==cur-1){
            //printf("P");
            if(cur&1){
                p1.init(scene,cur-1);
            }else{
                p0.init(scene,cur-1);
            }
            cur--;
        }
        if(t!=cur && t!=cur+1){
            //printf("J");
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
class worker{
    public:
    
    io_manager io;
    static void help(){
        std::cout<<"该功能暂无帮助"<<std::endl;
    }
    virtual void works()=0;

};

class worker_base:public worker{
    private:
    uint32_t cur;
    bool allow_negative;
    long long rest;
    uint32_t block;
    long long nxt_disp;
    void set(uint32_t begin,uint32_t uid,uint32_t mode,long long amt,bool neg){
        offset=uid+mode;
        cur=(begin+offset)*inv;
        rest=amt;
        allow_negative=neg;
        block=amt/100;
        nxt_disp=amt-block;
    }
    long long times;

    protected:
    
    uint32_t offset;
    int scene;
    
    virtual void init(){
        uint32_t seed,maxn,uid,mode;

        std::cout<<"起始种子(十六进制,默认为0): ";
        seed=io.get_hex(0,1ll<<32,0);
        
        std::cout<<"最大范围(默认计算所有种子): ";
        maxn=io.get_number(1,(1ll<<31)+1,1ll<<31);

        std::cout<<"场景(默认为泳池): ";
        scene=io.get_scene(POOL);

        std::cout<<"用户编号(默认为1): ";
        uid=io.get_number(0,0,1);
        
        std::cout<<"存档编号(默认为13): ";
        mode=io.get_number(0,0,13);

        set(seed,uid,mode,maxn,0);
    }
    virtual void disp(){
        std::cout<<
            "用时:"<<
            std::setiosflags(std::ios::fixed)<<std::setw(10)<<std::setprecision(3)<<times/1000.0<<
            "秒"<<
            std::endl;
    }
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
        uint32_t end=((cur>>d_page::LENGTH)+64)<<d_page::LENGTH;
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
    void works(){
        try{
            init();
        }catch(io_manager::io_exception const& e){
            std::cout<<"输入格式错误!"<<std::endl;
            return;
        }
        long long t1=clock();
        for(int i=0;i<100;i++){
            putchar('=');
        }
        putchar('\n');
        std::thread ls[TRDS];
        for(int i=0;i<TRDS;i++){
            ls[i]=std::thread([=](){
                work();
            });
        }
        for(int i=0;i<TRDS;i++){
            ls[i].join();
        }
        putchar('\n');
        long long t2=clock();
        times=t2-t1;
        disp();
    }
};

class worker_satisfy :public worker_base{
    protected:
    uint32_t result;
    bool found=0;
    virtual void init(){
        worker_base::init();
    }
    virtual void disp(){
        worker_base::disp();
        if(found){
            std::cout<<
                "成功找到种子: "<<
                std::setw(8)<<std::hex<<result<<std::dec<<
                std::endl;
        }else{
            std::cout<<"未能找到种子"<<std::endl;
        }
    }
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
    private:
    int begin,end,include[1<<10],exclude[1<<10];
    void init(){
        worker_satisfy::init();
        std::cout<<"起始旗帜数(奇数): ";
        begin=io.get_number();
        if((begin&1)!=1)throw io_manager::io_exception(0);
        
        std::cout<<"终止旗帜数(偶数), 直接输入回车则计算当前关: ";
        end=io.get_number(0,0,begin+1);
        if((end&1)!=0)throw io_manager::io_exception(0);

        begin/=2;
        end/=2;

        int inc=1;
        int exc=0;
        for(int i=begin;i<end;i++){
            std::cout<<std::setw(4)<<i*2+1<<" -"<<std::setw(4)<<i*2+2<<" flag 的信息"<<std::endl;
            std::cout<<"包含以下僵尸, 输入\"P\"则之后使用上一关的设置: ";
            try{
                inc=io.get_zombies();
            }catch(io_manager::io_exception const& s){
                if(s.value!=1)throw s;
                if(i==begin){
                    throw s;
                }
                for(int j=i;j<end;j++){
                    include[j]=inc;
                    exclude[j]=exc;
                }
                break;
            }

            std::cout<<"不包含以下僵尸, 输入回车则不作限制, 输入\"A\"则排除其它所有僵尸(除普僵): ";
            try{
                exc=io.get_zombies();
            }catch(io_manager::io_exception const& s){
                if(s.value!=2)throw s;
                exc=~inc;
            }
            exc&=0xfffffffe;
            include[i]=inc;
            exclude[i]=exc;
        }
    }
    void disp(){
        worker_satisfy::disp();
    }
    bool check(uint32_t i_seed,d_page& p){
        for(int i=begin;i<end;i++){
            int t;
            t=p.get(i_seed+i,i);
            if(t&exclude[i])return 0;
            if((t&include[i])!=include[i])return 0;
        }
        return 1;
    }
    public:
    static void help(){
        std::cout<<"功能: 给出每一关可以/不可以出现的怪, 筛选合适的种子"<<std::endl;
        std::cout<<"出怪情况格式如下"<<std::endl;
        std::cout<<"  1. 直接输入僵尸简称, 中间没有分隔"<<std::endl;
        std::cout<<"     示例: 车丑橄舞"<<std::endl;
        std::cout<<"  2. 输入僵尸名称, 中间有分隔"<<std::endl;
        std::cout<<"     僵尸可以输入全称/简称/别名"<<std::endl;
        std::cout<<"     分隔符可以为逗号、空格等"<<std::endl;
        std::cout<<"     示例: 路障 大爷,白 GIGA"<<std::endl;
        std::cout<<"场地可以输入中文或英文"<<std::endl;
        std::cout<<"   如: 泳池 POOL PE 均可识别"<<std::endl;
    }
};
class worker_limit :public worker_satisfy{
    private:
    int begin,end,type,limit;
    void init(){
        worker_satisfy::init();
        std::cout<<"起始旗帜数(奇数): ";
        begin=io.get_number();
        if((begin&1)!=1)throw io_manager::io_exception(0);
        
        std::cout<<"终止旗帜数(偶数), 直接输入回车则计算当前关: ";
        end=io.get_number(0,0,begin+1);
        if((end&1)!=0)throw io_manager::io_exception(0);

        
        std::cout<<"目标僵尸: ";
        type=io.get_single_zombie();

        std::cout<<"最大次数(默认值为0): ";
        limit=io.get_number(0,0,0);

        begin/=2;
        end/=2;
    }
    void disp(){
        worker_satisfy::disp();
    }
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
    public:
    static void help(){
        std::cout<<"功能: 给出某种怪在给定范围内最多出现的次数, 筛选合适的种子"<<std::endl;
        std::cout<<"僵尸可以输入全称/简称/别名"<<std::endl;
        std::cout<<"   如: 橄榄 橄 FOOTBALL 大爷 均可识别"<<std::endl;
        std::cout<<"场地可以输入中文或英文"<<std::endl;
        std::cout<<"   如: 泳池 POOL PE 均可识别"<<std::endl;
    }
};
class worker_minimum :public worker_base{
    protected:
    uint32_t result;
    long long best=(1ull<<63)-1;
    virtual void init(){
        worker_base::init();
    }
    virtual void disp(){
        worker_base::disp();
        
        std::cout<<
            "最优种子: "<<
            std::setw(8)<<std::hex<<result<<std::dec<<
            std::endl;
        std::cout<<
            "总权重"<<
            std::setw(8)<<best<<std::dec<<
            std::endl;
    
    }
    virtual long long get_value(uint32_t,d_page&)=0;
    
    private:
    void work(){
        d_page p(scene);
        long long r1=(1ull<<63)-1;
        uint32_t r2;
        while(1){
            mtx.lock();
            task t=get();
            mtx.unlock();
            if(t.begin==t.end)break;
            uint32_t cur=t.begin;
            while(cur!=t.end){
                long long x=get_value(cur,p);
                if(x<r1){
                    r1=x;
                    r2=cur*101-offset;
                }
                cur++;
            }
            mtx.lock();
            if(r1<best){
                best=r1;
                result=r2;
            }else{
                r1=best;
            }
            mtx.unlock();
        }
    }
};
class worker_weight :public worker_minimum{
    private:
    int begin,end;
    long long weight[1<<10][20];
    void init(){
        worker_minimum::init();
        std::cout<<"起始旗帜数(奇数): ";
        begin=io.get_number();
        if((begin&1)!=1)throw io_manager::io_exception(0);
        
        std::cout<<"终止旗帜数(偶数), 直接输入回车则计算当前关: ";
        end=io.get_number(0,0,begin+1);
        if((end&1)!=0)throw io_manager::io_exception(0);

        begin/=2;
        end/=2;

        for(int i=begin;i<end;i++){
            std::cout<<std::setw(4)<<i*2+1<<" -"<<std::setw(4)<<i*2+2<<" flag 的信息"<<std::endl;
            std::cout<<"僵尸权重, 输入\"P\"则之后使用上一关的设置: ";
            try{
                io.get_kv(weight[i]);
            }catch(io_manager::io_exception const& s){
                if(s.value!=1)throw s;
                if(i==begin){
                    throw s;
                }
                for(int j=i;j<end;j++){
                    std::copy(weight[i-1],weight[i-1]+20,weight[j]);
                }
                break;
            }
        }
    }
    void disp(){
        worker_minimum::disp();
    }
    long long get_value(uint32_t i_seed,d_page& p){
        long long value=0;
        for(int i=begin;i<end;i++){
            int t;
            t=p.get(i_seed+i,i);
            for(int j=0;j<20;j++){
                if(t&(1<<j))value+=weight[i][j];
            }
        }
        return value;
    }
    public:
    static void help(){
        std::cout<<"功能: 给出每一关每种怪的威胁度, 筛选威胁度最低的种子"<<std::endl;
        std::cout<<"威胁度格式如下"<<std::endl;
        std::cout<<"  1. 直接按顺序输入数字, 中间有分隔"<<std::endl;
        std::cout<<"     分隔符可以为逗号、空格等"<<std::endl;
        std::cout<<"     顺序为: ";
                    
        std::cout<<"普僵,路障,撑杆,铁桶,读报,铁门,橄榄,舞王,潜水,冰车,海豚,小丑,气球,矿工,跳跳,蹦极,扶梯,投篮,白眼,红眼"<<std::endl;
        std::cout<<"     示例: ,,,,,,5,,,10,2,3,5,2,,2,7,2,10,20"<<std::endl;
        std::cout<<"     也可以这样:  , ,0,,,,5 , , ,10 2 3,5,2,,2,7,2,10,20"<<std::endl;
        std::cout<<"     以上两种实际含义相同"<<std::endl;
        std::cout<<"     留空表示威胁度为0"<<std::endl;
        std::cout<<"  2. 输入僵尸名称, 后面跟威胁度, 两个不同僵尸的威胁度之间有分隔"<<std::endl;
        std::cout<<"     僵尸可以输入全称/简称/别名"<<std::endl;
        std::cout<<"     分隔符可以为逗号、空格等"<<std::endl;
        std::cout<<"     僵尸名称可以紧跟威胁度"<<std::endl;
        std::cout<<"     示例: 路障2 大爷 5"<<std::endl;
        std::cout<<"场地可以输入中文或英文"<<std::endl;
        std::cout<<"   如: 泳池 POOL PE 均可识别"<<std::endl;
    }
};
class worker_single:public worker{
    public:
    void works(){
        uint32_t seed;
        int uid,mode,scene,begin,end;
        uint32_t l=(seed+uid+mode)*inv;
        try{
            std::cout<<"种子(十六进制): ";
            seed=io.get_hex(0,1ll<<32,-1);
            
            std::cout<<"场景(默认为泳池): ";
            scene=io.get_scene(POOL);

            std::cout<<"用户编号(默认为1): ";
            uid=io.get_number(0,0,1);
            
            std::cout<<"存档编号(默认为13): ";
            mode=io.get_number(0,0,13);
            
            std::cout<<"起始旗帜数(奇数): ";
            begin=io.get_number();
            if((begin&1)!=1)throw io_manager::io_exception(0);
            
            std::cout<<"终止旗帜数(偶数), 直接输入回车则计算当前关: ";
            end=io.get_number(0,0,begin+1);
            if((end&1)!=0)throw io_manager::io_exception(0);

            begin/=2;
            end/=2;
        }catch(io_manager::io_exception const& e){
            std::cout<<"输入格式错误!"<<std::endl;
            return;
        }
        
        d_page p1(scene);
        for(int i=begin;i<end;i++){
            int t=p1.get(l+i,i);
            std::cout<<std::setw(4)<<i*2+1<<" - "<<std::setw(4)<<i*2+2<<" Flag: ";
            io.disp(t);
            std::cout<<std::endl;
        }
    }
    static void help(){

        std::cout<<"功能: 根据种子计算出怪"<<std::endl;
        std::cout<<"场地可以输入中文或英文"<<std::endl;
        std::cout<<"   如: 泳池 POOL PE 均可识别"<<std::endl;
    }
};
class worker_conv:public worker{
    public:
    void works(){
        uint32_t seed;
        int mode,uid,mode2,uid2;
        try{
            std::cout<<"原种子(十六进制): ";
            seed=io.get_hex(0,1ll<<32,-1);

            std::cout<<"原用户编号(默认为1): ";
            uid=io.get_number(0,0,1);
            
            std::cout<<"原存档编号(默认为13): ";
            mode=io.get_number(0,0,13);

            std::cout<<"新用户编号(默认为1): ";
            uid2=io.get_number(0,0,1);
            
            std::cout<<"新存档编号(默认为13): ";
            mode2=io.get_number(0,0,13);
        }catch(io_manager::io_exception const& e){
            std::cout<<"输入格式错误!"<<std::endl;
            return;
        }
        uint32_t seed2;
        seed2=seed+uid+mode-uid2-mode2;
        std::cout<<"种子: "<<std::setw(8)<<std::hex<<seed2<<std::dec<<std::endl;
        if(seed2>=(1ll<<31)){
            std::cout<<"警告: 种子不在有效范围内"<<std::endl;
        }
    }
    static void help(){
        std::cout<<"功能: 将一个存档的出怪种子换算到另一个种子"<<std::endl;
        std::cout<<"场地可以输入中文或英文"<<std::endl;
        std::cout<<"   如: 泳池 POOL PE 均可识别"<<std::endl;
    }
};
int main(){
    std::cout<<"注意:本软件默认单线程, 请在设置中修改(退出程序需要重新设置)"<<std::endl;
    while(1){
        
        std::cout<<"0. 查看帮助"<<std::endl;
        std::cout<<"1. 根据种子计算出怪"<<std::endl;
        std::cout<<"2. 按出怪求种子"<<std::endl;
        std::cout<<"3. 按单个怪的最大次数求种子"<<std::endl;
        std::cout<<"4. 按威胁值求种子"<<std::endl;
        std::cout<<"5. 换算种子"<<std::endl;
        std::cout<<"s. 设置"<<std::endl;
        std::cout<<"选择模式, 或按回车退出: ";
        std::string mode;
        std::getline(std::cin,mode);
        if(mode=="0"){
            io_manager io;
            std::cout<<"0. 简介"<<std::endl;
            std::cout<<"1. 根据种子计算出怪"<<std::endl;
            std::cout<<"2. 按出怪求种子"<<std::endl;
            std::cout<<"3. 按单个怪的最大次数求种子"<<std::endl;
            std::cout<<"4. 按威胁值求种子"<<std::endl;
            std::cout<<"5. 换算种子"<<std::endl;
            
            std::cout<<"n. 查看名称/简称/别名列表"<<std::endl;
            std::cout<<"q. 返回主菜单"<<std::endl;
            
            std::cout<<"请选择页面:";
            std::getline(std::cin,mode);
            if(mode=="0"){
                std::cout<<"作者: Hope2075"<<std::endl;
                std::cout<<"参考了星锦柒奈版计算器的功能"<<std::endl;
                std::cout<<""<<std::endl;
                std::cout<<"本软件可以根据种子计算出怪, 或给定条件筛选种子"<<std::endl;
                std::cout<<"默认为单线程, 如有需要可以进入设置页改为多线程"<<std::endl;
                std::cout<<"输入支持中文和英文, 请查看其它页面"<<std::endl;
                std::cout<<""<<std::endl;
                std::cout<<"如果出现不明原因报错/闪退, 或者计算结果错误"<<std::endl;
                std::cout<<"请向作者反馈"<<std::endl;
            }else if(mode=="1"){
                worker_single::help();
            }else if(mode=="2"){
                worker_type::help();
            }else if(mode=="3"){
                worker_limit::help();
            }else if(mode=="4"){
                worker_weight::help();
            }else if(mode=="5"){
                worker_conv::help();
            }else if(mode=="n"){
                io.list_names();
            }else if(mode=="q"){
                
            }else{
                std::cout<<"输入格式错误"<<std::endl;
            }
            continue;
        }else if(mode=="1"){
            worker_single wk;
            wk.works();
        }else if(mode=="2"){
            worker_type wk;
            wk.works();
        }else if(mode=="3"){
            worker_limit wk;
            wk.works();
        }else if(mode=="4"){
            worker_weight wk;
            wk.works();
        }else if(mode=="5"){
            worker_conv wk;
            wk.works();
                
        }else if(mode=="s"){
            io_manager io;
            std::cout<<"请输入线程数: ";
            try{
                int a;
                a=io.get_number(1,100,-1);
                TRDS=a;
                
                std::cout<<"线程数已设为"<<a<<std::endl;
            }catch(io_manager::io_exception const& e){
                std::cout<<"输入格式错误!"<<std::endl;
                continue;
            }
        }else if(mode==""){
            break;
        }else{
            std::cout<<"模式编号错误"<<std::endl;
        }
    }
}
