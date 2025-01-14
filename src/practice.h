#ifndef PRACTICE_H
#define PRACTICE_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t textureFlag;
    void * arcP;
    uint32_t startSelectEnable;
    int32_t screenSize; //in bytes
    uint16_t pastBright;
    uint16_t pastBright2;
    uint16_t pastBright3;
    uint16_t rng;
    uint8_t sigmaOvl;
    uint8_t reloadFlag;
    uint8_t page;
    bool made;
}State;


typedef struct{
    State state;
    uint8_t page;
    uint8_t sigmaOvl;
    uint8_t category;
    bool keepRng;
    uint8_t skipRefights;
    bool ultimateArmor;
    bool cancelXA;
}Practice;

extern Practice practice;

enum Categories {
    ALL_STAGES_OLD,
    ALL_STAGES,
    ANY_PERCENT,
    HUNDO,
    ALL_STAGES_UNARMORED,
    ANY_PERCENT_ULTIMATE,
    ANY_PERCENT_ZERO,
    MIN_XTREME,
    CUSTOM
};

#if BUILD == 1395
#define CONFIRM PAD_CROSS
#define CANCEL PAD_TRIANGLE
#else
#define CONFIRM PAD_CIRCLE
#define CANCEL PAD_CROSS
#endif

#endif