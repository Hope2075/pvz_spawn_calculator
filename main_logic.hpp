#ifndef MAIN_LOGIC_HPP_
#define MAIN_LOGIC_HPP_

#include <cstdio>
#include <string>

#include "page.hpp"
#include "string_split.hpp"
#include "interface.hpp"

inline void mode1()
{
    ::std::uint32_t seed;
    int mode, uid, scene, begin, end;

    seed = getseed();
    scene = getscene();
    uid = getuid();
    mode = getmode();
    begin = getbeginflag();
    end = getendflag(begin);

    calc_single(seed, uid, mode, scene, begin, end);
    // calc_single(0x09a6313b,1,13,POOL,0,40);
}

inline void mode2()
{
    ::std::uint32_t seed;
    int mode, uid, scene, begin, end;
    long long maxn;
    worker_type wk;

    seed = getseed();
    maxn = getseedmaxn();
    scene = getscene();
    uid = getuid();
    mode = getmode();
    begin = getbeginflag();
    end = getendflag(begin);

    getworkertypesettings(wk, begin, end);

    wk.set(seed, uid, mode, scene, maxn, 0);
    wk.works(8);
    returnworktime2(wk);
}

inline void mode3()
{
    ::std::uint32_t seed;
    int mode, uid, scene, begin, end;
    long long maxn;
    std::string input;
    worker_limit wk;
    int target, limit;

    seed = getseed();
    maxn = getseedmaxn();
    scene = getscene();
    uid = getuid();
    mode = getmode();
    begin = getbeginflag();
    end = getendflag(begin);

    wk.begin = begin;
    wk.end = end;

    target = getzombietarget();
    limit = getzombielimit(begin, end);

    wk.times = limit;
    wk.type = target;
    
    getworkerlimitsettings(wk); //<! 什么都不做, 只是为了和 mode3 对称

    // std::cout<<"DEBUG:"<<limit<<" "<<target<<std::endl;
    
    wk.set(seed, uid, mode, scene, maxn, 0);
    wk.works(8);
    returnworktime3(wk);
}

#endif