#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstdint>

constexpr ::std::uint32_t LENGTH { 10 };
constexpr ::std::uint32_t BLOCK { ::std::uint32_t(1) << LENGTH };
constexpr ::std::uint32_t MASK { BLOCK - 1 };
constexpr long long inv { 2083697005 }; //<! (uint32_t)(inv*101) == 1

enum scene  {
    DAY,
    NIGHT,
    POOL,
    FOG,
    ROOF,
    MOON,
    MUSHROOM,
    AQUARIOM
};

enum zombie {
    REGULAR,
    CONE,
    POLE,
    BUCKET,
    NEWS,
    DOOR,
    FOOTBALL,
    DANCE,
    SNORKEL,
    ZOMBONI,
    DOLPHIN,
    JACK,
    BALLOON,
    DIGGER,
    POGO,
    BUNGEE,
    LADDER,
    CATAPULT,
    GARG,
    GIGA
};

#endif