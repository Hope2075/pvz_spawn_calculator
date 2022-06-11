#ifndef PAGE_HPP_
#define PAGE_HPP_

#include <cstdint>

#include "constants.hpp"
#include "mt19937.hpp"

class d_page;

class page  {
    friend class d_page;
    int id;
    int scene;
    int data[BLOCK * 8];
    static constexpr ::std::size_t data_y_width = BLOCK;
    static constexpr ::std::size_t data_x_width = 8;
    static constexpr int allow_base[8] =
        {0x0f0ffaff, 0x0f0ff8ff, 0x0f0fffff, 0x0f0fffff,
         0x0f0fda7f, 0x0f0fda7f, 0x0f0ffaff, 0x0f0ffaff};
    static constexpr int conv_id[33] =
        {31, 24,  1,  2,  3,
          4,  5,  6,  7, 31,
         31,  8,  9, 31, 10,
         11, 12, 13, 14, 31,
         15, 16, 17, 18, 31,
         25, 31, 31, 31, 31,
         31, 31, 19};

public:
    void init(int sc, int idx)  {
        mt19937 g;
        int d1[10];
        int allow;
        this->scene = sc;
        this->id = idx;
        allow = this->allow_base[this->scene];
        uint32_t cur_seed = this->id * BLOCK;
        for (uint32_t i = 0; i < BLOCK; ++i)  {
            uint32_t rng_seed = cur_seed * 101;
            if (rng_seed == 0)
                rng_seed = 0x1105;
            g.seed(rng_seed);
            ++cur_seed;
            int has = 1;
            d1[0] = 1;
            if (g.modulo(5) == 0)  {
                d1[0] |= (1 << NEWS);
                has |= (1 << NEWS);
            } else  {
                d1[0] |= (1 << CONE);
                has |= (1 << CONE);
            }
            int j = 1;
            while (j < 10)  {
                int type = g.modulo(33);
                int nid = conv_id[type];
                // if(!(allow&(1<<nid)))continue;
                if (has & (1 << nid))
                    continue;
                if (allow & (1 << nid))
                {
                    has |= (1 << nid);
                    d1[j] = d1[j - 1];
                    if (nid != 24 && nid != 25)
                        d1[j] |= (1 << nid);
                    j++;
                }
            }
            for (j = 2; j < 6; ++j)
                if (d1[j] & (1 << GIGA))
                    d1[j] = d1[j + 1] ^ (1 << GIGA);
            if (d1[2] & (1 << ZOMBONI))  {
                if (d1[2] & (1 << GARG))
                    d1[2] = d1[4] ^ (1 << ZOMBONI) ^ (1 << GARG);
                else
                    d1[2] = d1[3] ^ (1 << ZOMBONI);
            }
            else
                if (d1[2] & (1 << GARG))
                    d1[2] = d1[3] ^ (1 << GARG);
            for (j = 0; j < 8; ++j)
                data[i*data_x_width + j] = d1[j + 2];
        }
    }
    int get(uint32_t pos, int level)
    {
        return data[pos*data_x_width + level];
    }
};

constexpr ::std::size_t page::data_x_width;
constexpr ::std::size_t page::data_y_width;
constexpr int page::allow_base[8];
constexpr int page::conv_id[33];

class d_page  {
    using uint32_t = ::std::uint32_t;
private:
    page p0, p1;
    int cur;
    int scene;
public:
    d_page()
        {}
    d_page(int sc)
    {
        scene = sc;
        p0.init(sc, 0);
        p1.init(sc, 1);
    }
    int get(uint32_t pos, int level)
    {
        if (level == 0)
            return 0b1011;
        --level;
        if (level >= 8)
            level = 7;
        uint32_t t = pos >> LENGTH;
        if (t == uint32_t(this->cur + 2))  {
            if (this->cur & 1)
                p1.init(scene, this->cur + 2);
            else
                p0.init(scene, this->cur + 2);
            ++cur;
        }
        if (t == uint32_t(cur - 1))  {
            if (this->cur & 1)
                p1.init(scene, this->cur - 1);
            else
                p0.init(scene, this->cur - 1);
            --cur;
        }
        if (t != uint32_t(this->cur) && t != uint32_t(this->cur + 1))  {
            if (t & 1)  {
                p1.init(scene, t);
                p0.init(scene, t + 1);
            } else  {
                p1.init(scene, t + 1);
                p0.init(scene, t);
            }
            cur = t;
        }
        if (t & 1)
            return p1.get(pos & MASK, level);
        else
            return p0.get(pos & MASK, level);
    }
};

#endif