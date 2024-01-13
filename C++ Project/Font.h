#include <stdio.h>

#define FONT_WIDTH 6
#define FONT_HEIGHT 5

typedef int Font[5][6];

Font font_array[26] =
    {
        // a
        {{1, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 1, 0},
         {0, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 1, 1, 1, 1, 0}},
        // b
        {{1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 1, 1, 1, 0, 0}},
        // c
        {{0, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0},
         {0, 1, 1, 1, 1, 0}},
        // d
        {{0, 0, 0, 0, 1, 0},
         {0, 0, 0, 0, 1, 0},
         {0, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 1, 1, 1, 1, 0}},
        // e
        {{0, 1, 1, 1, 0, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 1, 1, 1, 1, 0}},
        // f
        {{0, 0, 1, 1, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 1, 1, 1, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0}},
        // g
        {{1, 1, 1, 1, 0, 0},
         {1, 0, 0, 0, 1, 0},
         {0, 1, 1, 1, 1, 0},
         {0, 0, 0, 0, 1, 0},
         {0, 1, 1, 1, 1, 0}},
        // h
        {{1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0}},
        // i
        {{0, 0, 1, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0}},
        // j
        {{0, 0, 1, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 1, 1, 0, 0, 0}},
        // k
        {{1, 0, 0, 1, 0, 0},
         {1, 0, 1, 0, 0, 0},
         {1, 1, 0, 0, 0, 0},
         {1, 0, 1, 0, 0, 0},
         {1, 0, 0, 1, 0, 0}},
        // l
        {{0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0}},
        // m
        {{0, 1, 0, 1, 0, 0},
         {1, 0, 1, 0, 1, 0},
         {1, 0, 1, 0, 1, 0},
         {1, 0, 1, 0, 1, 0},
         {1, 0, 1, 0, 1, 0}},
        // n
        {{1, 0, 1, 1, 0, 0},
         {1, 1, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0}},
        // o
        {{0, 1, 1, 1, 0, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {0, 1, 1, 1, 0, 0}},
        // p
        {{0, 1, 1, 1, 0, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 1, 1, 1, 0, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0}},
        // q
        {{0, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {0, 1, 1, 1, 1, 0},
         {0, 0, 0, 0, 1, 0},
         {0, 0, 0, 0, 1, 0}},
        // r
        {{1, 0, 1, 1, 1, 0},
         {1, 1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0}},
        // s
        {{0, 1, 1, 1, 1, 0},
         {1, 0, 0, 0, 0, 0},
         {0, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 1, 0},
         {1, 1, 1, 1, 0, 0}},
        // t
        {{0, 0, 1, 0, 0, 0},
         {0, 1, 1, 1, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 1, 1, 0, 0}},
        // u
        {{1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {0, 1, 1, 1, 0, 0}},
        // v
        {{1, 0, 0, 0, 1, 0},
         {1, 0, 0, 0, 1, 0},
         {0, 1, 0, 1, 0, 0},
         {0, 1, 0, 1, 0, 0},
         {0, 0, 1, 0, 0, 0}},
        // w
        {{1, 0, 1, 0, 1, 0},
         {1, 0, 1, 0, 1, 0},
         {1, 1, 1, 1, 1, 0},
         {1, 1, 0, 1, 1, 0},
         {1, 0, 0, 0, 1, 0}},
        // x
        {{1, 0, 0, 0, 1, 0},
         {0, 1, 0, 1, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 1, 0, 1, 0, 0},
         {1, 0, 0, 0, 1, 0}},
        // y
        {{1, 0, 0, 0, 1, 0},
         {0, 1, 0, 1, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0}},
        // z
        {{1, 1, 1, 1, 1, 0},
         {0, 0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0, 0},
         {0, 1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1, 0}}};