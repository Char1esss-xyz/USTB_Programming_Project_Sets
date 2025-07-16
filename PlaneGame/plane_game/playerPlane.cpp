#include "playerPlane.h"
#include "bullet.h"
#include <conio.h>
#include<iostream>
playerPlane player;

//初始化玩家
playerPlane::playerPlane() : x(27), y(27) {}

//绘制玩家
void playerPlane::draw() {
    ConsoleOperator::gotoXY(x - 2, y - 1); std::cout << " /=\\ ";
    ConsoleOperator::gotoXY(x - 2, y);     std::cout << "<<*>>";
    ConsoleOperator::gotoXY(x - 1, y + 1); std::cout << "* * ";
}

//移动玩家
void playerPlane::move() {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
        case 'a': if (x > 2) x--; break;
        case 'd': if (x < gameMap::width - 3) x++; break;
        case 'w': if (y > 1) y--; break;
        case 's': if (y < gameMap::height - 2) y++; break;
        case ' ': Bullet::bullets.emplace_back(x, y - 1, true); break;
        }
    }
}