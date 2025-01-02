#include <common.h>
#include <gpu.h>
#include <object.h>
#include <layer.h>
#include "practice.h"

#if BUILD == 1395
#define OVL_ADDR 0x800e9860
#define LAYOUT_ADDR 0x80097438
#define INVERTED_CLUT_ADDR 0x80097798
#define RIDE_ARMOR_ADDR 0x800CD340
#define TEXT_INFO_ADDR 0x8008eaf0
#define TEXT_EXTRA_ADDR 0x800e33b0
#define BRIGHT_ADDR 0x80090D54
#define M_BRIGHT_ADDR 0x800CF850
#define EM_BRIGHT_ADDR 0x8009742c
#define AFTER_IMG_ADDR 0x800972f8
#define TBL_BRIGHT_ADDR 0x80090c90
/*BSS Addresses*/
#define ST01_BSS_ADDR OVL_ADDR + 0x0000E560
#define ST04_BSS_ADDR OVL_ADDR + 0x0000A7B8
#define ST06_BSS_ADDR OVL_ADDR + 0x0000D340
#define ST12_BSS_ADDR OVL_ADDR + 0x0000DF38
/*BSS Addresses of Refights*/
#define MAV6_BSS_ADDR 0x0x800FEEB0
#else
#define OVL_ADDR 0x800eae90
#define LAYOUT_ADDR 0x80098af8
#define INVERTED_CLUT_ADDR 0x80098E58
#define RIDE_ARMOR_ADDR 0x800CEA00
#define TEXT_INFO_ADDR 0x800901b0
#define TEXT_EXTRA_ADDR 0x800e49ec
#define BRIGHT_ADDR 0x80092414
#define M_BRIGHT_ADDR 0x800d0f10
#define EM_BRIGHT_ADDR 0x80098AEC
#define AFTER_IMG_ADDR 0x800989b8
#define TBL_BRIGHT_ADDR 0x80092350
/*BSS Addresses*/
#define ST01_BSS_ADDR OVL_ADDR + 0x0000E560
#define ST04_BSS_ADDR OVL_ADDR + 0x0000A7A8
#define ST06_BSS_ADDR OVL_ADDR + 0x0000D2CC
#define ST12_BSS_ADDR OVL_ADDR + 0x0000DF54
/*BSS Addresses of Refights*/
#define MAV6_BSS_ADDR 0x800fee3c
#endif

void *freeAddress[] = {
    INVERTED_CLUT_ADDR, // Inverted  Clut (unused)
    0x801F8304, // Just before Thread Stack Memory
    0x801E67DC, // End of ARC buffer
    0x801F6000, // Demo Buffer
    LAYOUT_ADDR, // Layout Buffer
    0x8000A000, // Kernal RAM
    0x8000C000, // Kernal RAM
    0x8000E640  // Kernal RAM
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
    RIDE_ARMOR_ADDR, // Ride Armor
    TEXT_INFO_ADDR, // Text Info
    TEXT_EXTRA_ADDR, // Extra Text Info
    BRIGHT_ADDR, // Brightness stuff
    M_BRIGHT_ADDR, // More Brightness stuff
    EM_BRIGHT_ADDR, // Also More Brightness stuff
    TBL_BRIGHT_ADDR, // Brightness Table thing
    AFTER_IMG_ADDR  // After Images
};

void *maverickRefightBssAddresses[] = {
    0x80100BF4,
    0x800FEDE4,
    0x800FE9A4,
    0x80100520,
    0x800FDC80,
    MAV6_BSS_ADDR,
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
    OVL_ADDR + 0x00012410, 0, // ST00
    ST01_BSS_ADDR, 0, // ST01
    OVL_ADDR + 0x0000DE8C, 0, // ST02
    OVL_ADDR + 0x0000AC1C, 0, // ST03
    ST04_BSS_ADDR, OVL_ADDR + 0x0000FFBC, // ST04
    OVL_ADDR + 0x0000CA90, 0, // ST05
    ST06_BSS_ADDR, 0, // ST06
    OVL_ADDR + 0x00013790, 0, // ST07
    OVL_ADDR + 0x00013CB0, 0, // ST08
    0, 0, // ST09
    0, 0, // ST0A
    0, 0, // ST0B
    0, OVL_ADDR + 0x0000D87C, // ST0C
    0, 0, // ST0D
    0, 0, // ST0E
    0, 0, // ST0F
    OVL_ADDR + 0x00013D34, 0, // ST10
    OVL_ADDR + 0x00010388, 0, // ST11
    OVL_ADDR + 0x0000DF38, 0, // ST12
    0, 0, // ST13
    0, 0, // ST14
    0, 0, // ST15
    0, 0  // ST16
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
    0x6624, // Just before Thread Stack Memory
    0x3824, // End of ARC buffer
    0x2000, // Demo Buffer
    0x0300, // Layout Buffer
    0x1900, // Kernal RAM
    0x1F00, // Kernal RAM
    0x02D0  // Kernal RAM
};

uint16_t addressesSize[] = { //0x13C14
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
    0xC,    // Brightness stuff
    8,      // More Brightness stuff
    8,      // Also More Brightness stuff
    0x40,   // Brightness Table thing
    0x120   // After Images
};
void * bonusBossAddresses[] = {
    0x800fe284,
    0x80102214,
    0x800fce48
};
uint16_t bonusBossAddressesSize[] = {
    0x18,
    0x04,
    0x20
};

#undef LAYOUT_ADDR
#undef INVERTED_CLUT_ADDR
#undef RIDE_ARMOR_ADDR
#undef TEXT_INFO_ADDR
#undef TEXT_EXTRA_ADDR
#undef BRIGHT_ADDR
#undef M_BRIGHT_ADDR
#undef EM_BRIGHT_ADDR
#undef AFTER_IMG_ADDR
#undef TBL_BRIGHT_ADDR
/*Undef BSS Addresses*/
#undef ST01_BSS_ADDR
#undef ST04_BSS_ADDR
#undef ST06_BSS_ADDR
#undef ST12_BSS_ADDR
/*Undef Refights BSS*/
#undef MAV6_BSS_ADDR