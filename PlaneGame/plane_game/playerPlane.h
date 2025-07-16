#pragma once
#include "consoleOperator.h"
#include "gameMap.h"
#include "bullet.h"

extern class playerPlane player;

class playerPlane {
public:
    int x, y;
    playerPlane();
    void draw();
    void move();
};