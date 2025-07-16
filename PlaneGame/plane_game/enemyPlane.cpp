#include "enemyPlane.h"
#include "gameMap.h"
#include "bullet.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

std::vector<enemyPlane> enemyPlane::enemies;

enemyPlane::enemyPlane(int x, int y) : x(x), y(y) {}

//���Ƶл�
void enemyPlane::draw() {
	ConsoleOperator::gotoXY(x - 2, y - 1);
	std::cout << "\\+/";
	ConsoleOperator::gotoXY(x - 1, y);
	std::cout << "|";
}

//�ƶ��л�λ��
void enemyPlane::move() {
	y++;
}

//�л�����
void enemyPlane::shoot() {
	Bullet::bullets.push_back(Bullet(x - 1, y + 1, false));
}

//����ȫ���л�״̬
void enemyPlane::updateAll() {
	for (auto it = enemies.begin(); it != enemies.end();) {
		it->move();

		if (rand() % 10 == 0) {
			it->shoot();
		}

		if (it->y > gameMap::height - 2) {
			it = enemies.erase(it);
		}
		else {
			++it;
		}
	}
}

//����ȫ���л�
void enemyPlane::drawAll() {
	for (auto& e : enemies) e.draw();
}

//������ɵл�
void enemyPlane::spawn() {
	if (enemies.size() < 5) {
		int randomX = rand() % (gameMap::width - 4) + 3;
		enemies.emplace_back(randomX, 1);
	}
}