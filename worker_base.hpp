#ifndef WORKER_BASE_HPP_
#define WORKER_BASE_HPP_

#include <cstdint>
#include <cstdio>
#include <mutex>
#include <thread>
#include <ctime>

#include "constants.hpp"
#include "page.hpp"

class worker_base  {
private:
    uint32_t cur;
    bool allow_negative;
    long long rest;
    uint32_t block;
    long long nxt_disp;

protected:
    uint32_t offset;
    int scene;
    class task  {
        public:
        uint32_t begin;
        uint32_t end;
        task(uint32_t b,uint32_t e)  {
            begin = b;
            end = e;
        }
    };
    std::mutex mtx;
    task get()  {
        if(rest == 0)
            return task(0, 0);
        uint32_t begin = cur;
        uint32_t end = ((cur>>LENGTH)+64)<<LENGTH;
        cur = end;
        if(!allow_negative)  {
            uint32_t s1 = end*101-offset;
            if(s1 >= 0x80000000) {
                uint32_t delta = s1-0x80000000;
                end -= delta/101;
                cur += (uint32_t)(-s1+100)/101;
            }
        }
        uint32_t length = end-begin;
        if(length >= this->rest) {
            end = begin+rest;
            length = rest;
        }
        this->rest -= length;
        if(this->rest <= this->nxt_disp) {
            putchar('.');
            this->nxt_disp -= this->block;
        }
        return task(begin, end);
    }
    virtual void work() = 0;

    public:
    void set(uint32_t begin, uint32_t uid, uint32_t mode, int sc, long long amt, bool neg)  {
        offset = uid+mode;
        cur = (begin+offset)*inv;
        rest = amt;
        allow_negative = neg;
        block = amt/100;
        nxt_disp = amt-block;
        scene = sc;
    }
    long long times;
    void works(int cnt) {
        long long t1 = clock();
        for(int i = 0; i < 100; ++i) {
            putchar('=');
        }
        putchar('\n');
        std::thread ls[cnt];
        for(int i = 0; i < cnt; ++i) {
            ls[i]=std::thread([=](){
                work();
            });
        }
        for(int i = 0; i < cnt; i++) {
            ls[i].join();
        }
        putchar('\n');
        long long t2 = clock();
        this->times = t2-t1;
    }
};

class worker_satisfy : public worker_base {
public:
    uint32_t result;
    bool found;
protected:
    virtual bool check(uint32_t, d_page&) = 0;
    
    private:
    void work() {
        d_page p(scene);
        bool r1 = false;
        uint32_t r2;
        while(true) {
            mtx.lock();
            task t = get();
            mtx.unlock();
            if(t.begin == t.end)
                break;
            uint32_t cur = t.begin;
            while(cur != t.end){
                if(check(cur,p)) {
                    r1 = true;
                    r2 = cur;
                    break;
                }
                ++cur;
            }
            mtx.lock();
            if(r1){
                if(!this->found) {
                    found = true;
                    result = r2*101-this->offset;
                }
            }
            if(found) {
                mtx.unlock();
                break;
            }
            mtx.unlock();
        }
    }
};

class worker_type : public worker_satisfy {
public:
    int begin, end, include[1<<10], exclude[1<<10];
private:
    bool check(uint32_t i_seed, d_page& p) {
        for(int i = begin; i < end; ++i) {
            int t;
            t = p.get(i_seed+i,i);
            if(t & exclude[i])
                return false;
            if((t & include[i]) != include[i])
                return false;
        }
        return true;
    }
};
class worker_limit : public worker_satisfy {
    public:
    int begin, end, type, limit;
    private:
    bool check(uint32_t i_seed, d_page& p) {
        int cnt = 0;
        for(int i = begin; i < end; ++i) {
            int t;
            t = p.get(i_seed+i, i);
            if(t & (1<<type))
                ++cnt;
            if(cnt > limit)
            return false;
        }
        return true;
    }
};

#endif