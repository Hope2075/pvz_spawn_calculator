#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>

#include "constants.hpp"
#include "page.hpp"
#include "worker_base.hpp"
#include "string_split.hpp"
#include "interface.hpp"
#include "main_logic.hpp"


//FILE* f;

//long long cnt;
//std::mutex mc;

int main() {
using ::std::cout;
using ::std::cerr;
using ::std::endl;
using ::std::cin;
    while(true) {
    try{
        cout << "1. 根据种子计算出怪" << endl;
        cout << "2. 按每关的出怪求出种子" << endl;
        cout << "3. 限制单个怪的次数，求出种子" << endl;
        cout << "选择模式，或按回车退出: ";
        std::string mode;
        std::getline(cin, mode);
        if(mode == "1") {
            mode1();
        } else if(mode == "2")  {
            mode2();
        } else if(mode == "3")  {
            mode3();
        } else if(mode == "")  {
            throw 114514;
        } else  {
            std::cerr << "模式编号错误。\n";
        }
    } catch(std::exception const& e) {
        std::cerr << e.what() << '\n';
    } catch(int i) {
        if(114514 == i)  {
            std::cerr << "程序即将关闭。\n";
            break;
        }
        else {
            std::cerr << "Why can you throw an non-internal int exception?\n";
            break;
        }
    }
    }
}
