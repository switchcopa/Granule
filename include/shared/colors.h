// colors.h

#pragma once
#include <stdint.h>

typedef struct {
    uint8_t r, g, b;
} CellColor;

extern const CellColor sand_colors[3];
extern const CellColor wet_sand_colors[3];
extern const CellColor water_color;
extern const CellColor steam_color;
extern const CellColor stone_colors[3];
