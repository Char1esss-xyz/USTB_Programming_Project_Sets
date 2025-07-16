#pragma once
#include <vector>
#include "consoleOperator.h"

class enemyPlane {
public:
    int x, y;
    static std::vector<enemyPlane> enemies;

    enemyPlane(int x, int y);
    void draw();
    void move();
    static void updateAll();
    static void drawAll();
    static void spawn();
    void shoot();
};