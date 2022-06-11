#ifndef MT19937_HPP_
#define MT19937_HPP_

#include <cstdint>
#include <type_traits>

 /**
   * @brief A specialized feedback shift register discrete random number generator.
   *
   * This algorithm was originally invented by Makoto Matsumoto and
   * Takuji Nishimura. This generator will produce numbers with a very long period
   * and fairly good apparent entropy, although still not cryptographically strong.
   *
   * @param buffer[state_size] contains the matrix
   * @param state notes the state of twistion .
   */
class mt19937  {
public:
    /// 返回随机数的类型。
    using result_type = ::std::uint_fast32_t;
    using size_type = ::std::size_t;

    // 基本参数。与生成器内存有关。
    static constexpr size_type   word_size                 = 32;  //!< The number of bit in each element of the state vector.
    static constexpr size_type   state_size                = 624; //!< The degree of recursion.
    static constexpr size_type   shift_size                = 397; //!< The period parameter.
    static constexpr size_type   mask_bits                 = 31;  //!< The separation point bit index.
    static constexpr result_type xor_mask                  = 0x9908b0dfUL; //!< The last row of the twist matrix.

private:
    result_type buffer[state_size];
    size_type   state;

    static constexpr result_type upper_mask = (~result_type()) << mask_bits;
    static constexpr result_type lower_mask = ~upper_mask;
    /**
     * @brief 仅旋转 state 位置一个状态。
     */
    void twist_one_state() noexcept {
        // 没用，但这样写很好看（
        result_type const state2 = buffer[(state+1)%state_size];
        result_type const state3 = buffer[(state+shift_size)%state_size];

        // 编译器用我看不懂的方式优化掉了 % 取余数运算符……
        result_type const temp =
            (buffer[state] & upper_mask) | (state2 & lower_mask);
        // 没用，编译器已经把两个相同的 buffer[state] 优化掉了……
        buffer[state] =
            state3 ^ (temp >> 1);
        // 这个没法用 LH 法优化……
        if(temp & result_type(1))
            buffer[state] ^= xor_mask;
    }
    

    /**
     * @brief 按顺序旋转 [state, state+index) 位置各一个状态
     * 
     * @remark 请不要在类外重载该函数！！！因此我把这个模板设置为 private
     * 
     */
    template<size_type index, typename std::enable_if<(index > 0 && index < state_size), void*>::type = nullptr>
    void twist_several_state() noexcept {
        size_t da_state = state;
        size_t const de_state = (state+index)%state_size;
        do {
            result_type temp =
                (buffer[da_state] & upper_mask) | (buffer[(da_state+1) % state_size] & lower_mask);
            buffer[state] =
                (buffer[(da_state+shift_size) % state_size]) ^ (temp >> 1);
            if(temp & (result_type)1)
                buffer[da_state] ^= xor_mask;
            ++da_state;
            da_state %= state_size;
        } while(de_state != da_state);

    }

    /**
     * @brief 自 state 位置开始旋转一整周，即循环 state_size 次。
     */
    void twist_one_round() noexcept {
        size_t da_state = state;
        size_t const de_state = state;
        do {
            result_type temp =
                (buffer[da_state] & upper_mask) | (buffer[(da_state+1) % state_size] & lower_mask);
            buffer[state] =
                (buffer[(da_state+shift_size) % state_size]) ^ (temp >> 1);
            if(temp & result_type(1))
                buffer[da_state] ^= xor_mask;
            ++da_state;
            da_state %= state_size;
        } while(de_state != da_state);
    }

public:
    // 输出和初始化需要用到的参数。
    static constexpr size_t      tempering_u               = 11;           //!< The first right-shift tempering matrix parameter.
    static constexpr result_type tempering_d               = 0xffffffffUL; //!< redundant
    static constexpr size_t      tempering_s               = 7;            //!< The first left-shift tempering matrix parameter.
    static constexpr result_type tempering_b               = 0x9d2c5680UL; //!< The first left-shift tempering matrix mask.
    static constexpr size_t      tempering_t               = 15;           //!< The second left-shift tempering matrix parameter.
    static constexpr result_type tempering_c               = 0xefc60000UL; //!< The second left-shift tempering matrix mask.
    static constexpr size_t      tempering_l               = 18;           //!< The second right-shift tempering matrix parameter.
    static constexpr result_type initialization_multiplier = 1812433253UL; //!< The initialization multiplier.
    static constexpr result_type default_seed = 5489u;          //!< The default seed.

    // constructors and member functions
    mt19937()
        : mt19937(default_seed) { }

    explicit
    mt19937(result_type __sd)
        { seed(__sd); }

    /**
     * @brief 从 __sd 设定初始随机状态。
     * 原有状态清空。
     * 
     * @param __sd 种子
     */
    void
    seed(result_type __sd = default_seed) noexcept  {
        buffer[0] = __sd;
        size_t i = 1;
        do {
            buffer[i] = (buffer[i-1] ^ (buffer[i-1] >> (word_size - 2))) * initialization_multiplier + i;
            ++i;
        // 这种完全在 gcc 的优化范围内…… 写什么都无所谓
        } while(i < state_size);
        state = 0;
    }

    /**
     * @brief 返回可输出最小值。
     * @return 0
     */
    static constexpr result_type
    min() noexcept
        { return 0; }

    /**
     * @brief 返回可输出最大值。
     * @return result_type(-1), 即 0xFFFFFFFFu。
     */
    static constexpr result_type
    max() noexcept
        { return -1; }

    /**
     * @brief 连续丢弃 __z 个随机数状态。
     * 
     * @param __z 丢弃的状态数。
     */
    void
    discard(unsigned long long __z) {
        // 减少了 ++state 和 --__z 的脑瘫操作
        while(__z > state_size)  {
            __z -= state_size;
            twist_one_round();
        }
        while(__z)  {
            --__z;
            twist_one_state();
            ++state;
            if(state_size == state)  state = 0;
        }
    }

    /**
     * @brief Get one random numbers after state steps one forward. 
     * @return the random number.
     */
    result_type
    operator()() noexcept {
        twist_one_state();
        result_type temp = buffer[state++];
	    temp ^= (temp >> tempering_u);  //<! Actually it's temp ^= (temp >> tempering_u) & tempering_d;
	    temp ^= (temp << tempering_s) & tempering_b;
	    temp ^= (temp << tempering_t) & tempering_c;
	    temp ^= (temp >> tempering_l);
        if(state_size == state)  state = 0;
        return temp;
    }

    result_type
    modulo(result_type modulus) {
        return (this->operator()() & result_type(0x7fffffff)) % modulus;
    }

    friend bool
    operator==(const mt19937& __lhs,
	    const mt19937& __rhs) = delete;
};

#endif