#include "bullet.h"
#include "gameMap.h"
#include <iostream>
std::vector<Bullet> Bullet::bullets;

Bullet::Bullet(int x, int y, bool isPlayer) : x(x), y(y), isPlayer(isPlayer) {}

void Bullet::move() {
    if (isPlayer) {
        y -= 2; // 玩家子弹向上移动
    }
    else {
        y += 2; // 敌机子弹向下移动
    }
}

//绘制子弹
void Bullet::draw() {
    ConsoleOperator::gotoXY(x, y);
    std::cout << "*";
}

//更新子弹位置
void Bullet::updateAll() {
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->move();
        if (it->y <= 0 || it->y >= gameMap::height - 1) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

//遍历更新所有子弹位置
void Bullet::drawAll() {
    for (auto& b : bullets) {
        b.draw();
    }
}