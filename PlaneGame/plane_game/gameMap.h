#pragma once
#include <windows.h>

class gameMap {
public:
    static const int width = 55;
    static const int height = 35;

    static int score;
    static bool gameOver;

    static void drawBorder();
    static void printScore();
};