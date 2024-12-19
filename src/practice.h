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
    uint8_t route;
    bool keepRng;
    uint8_t skipRefights;
    bool ultimateArmor;
    bool cancelXA;
}Practice;

extern Practice practice;

#endif