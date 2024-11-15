#include <common.h>
#include <gpu.h>
#include <object.h>
#include <layer.h>
#include "practice.h"


void *freeAddress[] = {
    0x80097798, // Inverted  Clut (unused)
    0x801F8304, // Just before Thread Stack Memory
    0x801E4658, // End of ARC buffer
    0x801029B4, // End of Level Overlay
    0x80097438, // Layout Buffer
    0x8000A000, // Kernal RAM
    0x8000E440  // Kernal RAM
};

void *readAddress[] = {
    -1,         // Clut (dynamicly set based off of CLUT Pointer)
    WEAPON_OBJECT_ADDR, // Weapon Objects
    ENEMY_OBJECT_ADDR, // Main Objects
    SHOT_OBJECT_ADDR, // Shot Objects
    VISUAL_OBJECT_ADDR, // Visual Objects
    EFFECT_OBJECT_ADDR, // Effect Objects
    ITEM_OBJECT_ADDR, // Item Objects
    MISC_OBJECT_ADDR, // Misc Objects
    QUAD_OBJECT_ADDR, // Quad Objects
    LAYER_OBJECT_ADDR, // Layer Objects
    &mega, // Mega
    &bgLayers, // BG Layers
    &game, // Game Info
    0x800CD340, // Ride Armor
    0x8008EAf0, // Text Info
    0x800E33B0, // Extra Text Info
    0x80090D54, // Brightness stuff
    0x800CF850, // More Brightness stuff
    0x8009742C, // Also More Brightness stuff
    0x80090C90, // Brightness Table thing
    0x800972F8  // After Images
};

void *maverickRefightBssAddresses[] = {
    0x80100BF4,
    0x800FEDE4,
    0x800FE9A4,
    0x80100520,
    0x800FDC80,
    0x800FEEB0,
    0x80100D60,
    0x800FFE18
};

uint16_t maverickRefightBssSizes[] = {
    0x710,
    0x4,
    0x18,
    0x14,
    0xC,
    0x2C,
    0x8,
    0x14
};


void *stageBssAddresses[] = {
    0x800FBC70,0, //ST00
    0x800F7DC0,0, //ST01
    0x800F76EC,0, //ST02
    0x800F447C,0, //ST03
    0x800F4018,0x800F981C, //ST04
    0x800F62F0,0, //ST05
    0x800F6BA0,0, //ST06
    0x800FCFEC,0, //ST07
    0x800FD510,0, //ST08
    0,0, //ST09
    0,0, //ST0A
    0,0,          //ST0B
    0,0x800F70DC,          //ST0C
    0,0,          //ST0D
    0,0,          //ST0E
    0,0,          //ST0F
    0x800FD590,0, //ST10
    0x800F9bE8,0,          //ST11
    0x800F7798,0,  //ST12
    0,0,  //ST13
    0,0,  //ST14
    0,0,  //ST15
    0,0  //ST16
};

uint16_t *stageBssSizes[] = {
    0x10,0, //ST00
    0x710,0, //ST01
    0x158,0, //ST02
    0x18,0, //ST03
    0x8,0x60, //ST04
    0x80C,0, //ST05
    0x70,0, //ST06
    0x1C,0, //ST07
    0x578,0, //ST08
    0,0, //ST09
    0,0, //ST0A
    0,0,    //ST0B
    0,0xC,    //ST0C
    0,0,    //ST0D
    0,0,    //ST0E
    0,0,    //ST0F
    0x18,0, //ST10
    0x4,0, //ST11
    0x72,0, //ST12
    0,0, //ST13
    0,0, //ST14
    0,0, //ST15
    0,0 //ST16
};

uint16_t freeAddressSizes[] = {
    0x4000, // Inverted  Clut (unused)
    0x65FC, // Just before Thread Stack Memory
    0x59A8, // End of ARC buffer
    0x064C, // End of Level Overlay
    0x0300, // Layout Buffer
    0x1900, // Kernal RAM
    0x1A00  // Kernal RAM
};

uint16_t addressesSize[] = { //0x13BF0
    0x4000, // Clut
    WEAPON_OBJECT_SLOTS * WEAPON_OBJECT_SLOT_SIZES, // Weapon Objects
    ENEMY_OBJECT_SLOTS * ENEMY_OBJECT_SLOT_SIZES,   // Main Objects
    SHOT_OBJECT_SLOTS * SHOT_OBJECT_SLOT_SIZES,     // Shot Objects
    VISUAL_OBJECT_SLOTS * VISUAL_OBJECT_SLOT_SIZES, // Visual Objects
    EFFECT_OBJECT_SLOTS * EFFECT_OBJECT_SLOT_SIZES, // Effect Objects
    ITEM_OBJECT_SLOTS * ITEM_OBJECT_SLOT_SIZES,     // Item Objects
    MISC_OBJECT_SLOTS * MISC_OBJECT_SLOT_SIZES,     // Misc Objects
    QUAD_OBJECT_SLOTS * QUAD_OBJECT_SLOTS_SIZES,    // Quad Objects
    LAYER_OBJECT_SLOTS * LAYER_OBJECT_SLOT_SIZES,   // Layer Objects
    0x158,  // Mega
    0xFC,   // BG Layers
    0x468,  // Game Info
    0xC0,   // Ride Armor
    0x18,   // Text Info
    0x104,  // Extra Text Info
    8,      // Brightness stuff
    4,      // More Brightness stuff
    4,      // Also More Brightness stuff
    0x28,   // Brightness Table thing
    0x120   // After Images
};
