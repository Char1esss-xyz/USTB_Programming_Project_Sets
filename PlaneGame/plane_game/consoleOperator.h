#pragma once
#include <windows.h>

class ConsoleOperator {
public:
    static void hideCursor();
    static void gotoXY(int x, int y);
    static void clearScreen();
};