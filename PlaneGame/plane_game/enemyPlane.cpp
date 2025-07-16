#include "enemyPlane.h"
#include "gameMap.h"
#include "bullet.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

std::vector<enemyPlane> enemyPlane::enemies;

enemyPlane::enemyPlane(int x, int y) : x(x), y(y) {}

//绘制敌机
void enemyPlane::draw() {
	ConsoleOperator::gotoXY(x - 2, y - 1);
	std::cout << "\\+/";
	ConsoleOperator::gotoXY(x - 1, y);
	std::cout << "|";
}

//移动敌机位置
void enemyPlane::move() {
	y++;
}

//敌机攻击
void enemyPlane::shoot() {
	Bullet::bullets.push_back(Bullet(x - 1, y + 1, false));
}

//更新全部敌机状态
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

//绘制全部敌机
void enemyPlane::drawAll() {
	for (auto& e : enemies) e.draw();
}

//随机生成敌机
void enemyPlane::spawn() {
	if (enemies.size() < 5) {
		int randomX = rand() % (gameMap::width - 4) + 3;
		enemies.emplace_back(randomX, 1);
	}
}