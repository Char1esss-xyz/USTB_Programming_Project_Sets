#include "bullet.h"
#include "gameMap.h"
#include <iostream>
std::vector<Bullet> Bullet::bullets;

Bullet::Bullet(int x, int y, bool isPlayer) : x(x), y(y), isPlayer(isPlayer) {}

void Bullet::move() {
    if (isPlayer) {
        y -= 2; // ����ӵ������ƶ�
    }
    else {
        y += 2; // �л��ӵ������ƶ�
    }
}

//�����ӵ�
void Bullet::draw() {
    ConsoleOperator::gotoXY(x, y);
    std::cout << "*";
}

//�����ӵ�λ��
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

//�������������ӵ�λ��
void Bullet::drawAll() {
    for (auto& b : bullets) {
        b.draw();
    }
}