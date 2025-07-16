#pragma once
#include <vector>
#include "consoleOperator.h"

class Bullet {
public:
    int x, y;
    bool isPlayer; // true 表示是玩家子弹，false 表示敌机子弹

    static std::vector<Bullet> bullets;

    Bullet(int x, int y, bool isPlayer); 
    void move();
    void draw();
    static void updateAll();
    static void drawAll();
};