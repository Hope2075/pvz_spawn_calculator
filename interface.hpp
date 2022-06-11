#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "page.hpp"
#include "string_split.hpp"

inline void disp(int x) {
using ::std::cout;
    bool f = false;
    for(int i = 0; i < 20; ++i) {
        if(x & (1<<i)) {
            if(f) {
                cout << ", ";
            }
            f = true;
            cout << zombie_desc[i][0];
        }
    }
}

inline void calc_single(uint32_t seed, int uid, int mode, int scene, int begin, int end) {
using ::std::cout;  using ::std::endl;  using ::std::setw;
    d_page p1(scene);
    ::std::uint32_t l = (seed + uid + mode) * inv;
    //p1.reset(scene,l);
    for(int i = begin; i < end; ++i) {
        int t = p1.get(l+i,i);
        cout << setw(4) << i*2+1 << " - " << setw(4) << i*2+2 <<" Flag: ";
        disp(t);
        cout << endl;
    }
}

inline uint32_t getseed()
{
    using std::cin;
    using std::cout;
    std::string input{};
    ::std::uint32_t seed{};
    cout << "种子(十六进制)(默认为0): ";
    std::getline(cin, input);
    try
    {
        if(input == "")
            return 0x0;
        long long s0 = std::stoll(input, 0, 16);
        if (s0 < 0 || s0 >= (1ll << 32))
            throw std::out_of_range("");
        seed = s0;
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("种子格式错误！");
    }
    catch (std::out_of_range const &e)
    {
        throw std::out_of_range("种子数值非法！");
    }
    return seed;
}

inline long long getseedmaxn()
{
    std::string input{};
    long long maxn{};
    std::cout << "最大范围(默认计算所有种子): ";
    std::getline(std::cin, input);
    try
    {
        if (input == "")
        {
            maxn = 1ll << 31;
        }
        else
        {
            maxn = std::stol(input, 0, 10);
            if (maxn <= 0 || maxn > (1ll << 31))
                throw std::out_of_range("");
        }
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("最大范围格式错误！");
    }
    catch (std::out_of_range const &e)
    {
        throw std::out_of_range("最大范围数值非法！");
    }
    return maxn;
}

inline int getscene()
{
    using std::cin;
    using std::cout;
    std::string input{};
    int scene{};
    cout << "场景(默认为泳池): ";
    std::getline(cin, input);
    if (input == "")
    {
        scene = POOL;
    }
    else
    {
        try
        {
            scene = get_scene(input);
        }
        catch (std::invalid_argument const &e)
        {
            throw std::invalid_argument("场景格式错误！");
        }
    }
    return scene;
}

inline int getuid()
{
    std::string input{};
    int uid{};
    std::cout << "用户编号(默认为1): ";
    std::getline(std::cin, input);
    if (input == "")
    {
        uid = 1;
    }
    else
    {
        try
        {
            uid = std::stoul(input, 0, 10);
        }
        catch (std::invalid_argument const &e)
        {
            throw std::invalid_argument("用户编号格式错误！");
        }
        catch (std::out_of_range const &e)
        {
            throw std::out_of_range("用户编号数值非法！");
        }
    }
    return uid;
}

inline int getmode()
{
    using std::cin;
    using std::cout;
    std::string input{};
    int mode{};
    cout << "存档编号(默认为13): ";
    std::getline(cin, input);
    if (input == "")
    {
        mode = 13;
    }
    else
    {
        try
        {
            mode = std::stoul(input, 0, 10);
        }
        catch (std::invalid_argument const &e)
        {
            throw std::invalid_argument("存档编号格式错误！");
        }
        catch (std::out_of_range const &e)
        {
            throw std::out_of_range("存档编号数值非法！");
        }
    }
    return mode;
}

inline int getbeginflag()
{
    using std::cin;
    using std::cout;
    std::string input{};
    int begin{};
    cout << "起始旗帜数(奇数): ";
    std::getline(cin, input);
    try
    {
        begin = std::stol(input, 0, 10);
        if (begin < 0 || begin % 2 == 0)
            throw std::out_of_range("");
        begin /= 2;
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("起始旗帜数格式错误！");
    }
    catch (std::out_of_range const &e)
    {
        throw std::out_of_range("起始旗帜数数值非法！");
    }
    return begin;
}

inline int getendflag(int begin)
{
    using std::cin;
    using std::cout;
    std::string input{};
    int end{};
    cout << "终止旗帜数(偶数)，直接输入回车则计算当前关: ";
    std::getline(std::cin, input);
    try
    {
        if (input == "")
        {
            end = begin + 1;
        }
        else
        {
            end = std::stol(input, 0, 10);
            if (end < 0 || end % 2 == 1)
                throw std::out_of_range("");
            end /= 2;
            if (end <= begin)
                throw std::out_of_range("");
        }
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("终止旗帜数格式错误！");
    }
    catch (std::out_of_range const &e)
    {
        throw std::out_of_range("终止旗帜数数值非法！");
    }
    return end;
}

inline int getzombietarget() {
    std::string input{};
    int target{};
    std::cout << "目标僵尸: ";
    std::getline(std::cin, input);
    try
    {
        target = get_single_zombie(input);
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("目标僵尸种类格式错误！");
    }
    return target;
}

inline int getzombielimit(int begin, int end) {
    std::string input{};
    int limit{};
    std::cout << "最大次数(默认值为0): ";
    std::getline(std::cin, input);
    try
    {
        if (input == "")
        {
            limit = 0;
        }
        else
        {
            limit = std::stol(input, 0, 10);
            if (limit < 0 || begin + limit > end)
                throw std::out_of_range("");
        }
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("最大次数格式错误！");
    }
    catch (std::out_of_range const &e)
    {
        throw std::out_of_range("最大次数数值非法！");
    }
    return limit;
}

inline void getworkertypesettings(worker_type& wk, int begin, int end)
{
    try
    {
        wk.begin = begin;  wk.end = end;
        std::string input{};
        int include = 1;
        int exclude = 0;
        for (int i = begin; i < end; i++)
        {
            std::cout << std::setw(4) << i * 2 + 1 << " -" << std::setw(4) << i * 2 + 2 << " flag 的信息" << std::endl;
            std::cout << "包含以下僵尸，输入\"P\"则之后使用上一关的设置：";
            std::getline(std::cin, input);
            if (input == "P")
            {
                for (int j = i; j < end; j++)
                {
                    wk.include[j] = include;
                    wk.exclude[j] = exclude;
                }
                break;
            }
            include = get_zombies(input);
            std::cout << "不包含以下僵尸，输入回车则不作限制，输入\"A\"则排除其它僵尸：";
            std::getline(std::cin, input);
            if (input == "A")
            {
                exclude = ~include;
            }
            else
            {
                exclude = get_zombies(input);
            }
            exclude &= 0xfffffffe;
            wk.include[i] = include;
            wk.exclude[i] = exclude;
        }
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("输入格式错误！");
    }
}

inline void getworkerlimitsettings(worker_limit& wk) {
    return;
}

inline void returnworktime2(worker_satisfy& wk) {
    std::cout << "用时:" << std::setw(10) << std::setprecision(3) << wk.times / 1000.0 << "秒" << std::endl;

    if (wk.found)
    {
        std::cout << "成功找到种子：" << std::setw(8) << std::hex << wk.result << std::dec << std::endl;
    }
    else
    {
        std::cout << "未能找到种子" << std::endl;
    }
}

inline void returnworktime3(worker_satisfy& wk) {
    std::cout << "用时:" << std::setw(10) << std::setiosflags(std::ios::fixed) << std::setprecision(3) << wk.times / 1000.0 << "秒" << std::endl;
    if (wk.found)
    {
        std::cout << "成功找到种子：" << std::setw(8) << std::hex << wk.result << std::dec << std::endl;
    }
    else
    {
        std::cout << "未能找到种子" << std::endl;
    }
}

#endif