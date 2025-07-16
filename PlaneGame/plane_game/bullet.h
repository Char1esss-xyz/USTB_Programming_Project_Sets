#pragma once
#include <vector>
#include "consoleOperator.h"

class Bullet {
public:
    int x, y;
    bool isPlayer; // true ��ʾ������ӵ���false ��ʾ�л��ӵ�

    static std::vector<Bullet> bullets;

    Bullet(int x, int y, bool isPlayer); 
    void move();
    void draw();
    static void updateAll();
    static void drawAll();
};