#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>          
#include <windows.h>
#include "consoleOperator.h"
#include "gameMap.h"
#include "playerPlane.h"
#include "bullet.h"
#include "enemyPlane.h"

int main() {
	// ���ع�꣬����
	ConsoleOperator::hideCursor();
	ConsoleOperator::clearScreen();

	// ��ʼ��ȫ����Ϸ״̬
	gameMap::score = 0;
	gameMap::gameOver = false;

	while (!gameMap::gameOver) {
		Sleep(100);

		player.move();

		// ����
		Bullet::updateAll();
		enemyPlane::updateAll();


		if (rand() % 10 == 0) {
			enemyPlane::spawn();
		}

		//�ӵ��͵л���ײ���
		for (auto bIt = Bullet::bullets.begin(); bIt != Bullet::bullets.end();) {
			if (bIt->isPlayer) {
				bool hit = false;
				for (auto eIt = enemyPlane::enemies.begin(); eIt != enemyPlane::enemies.end(); ++eIt) {
					if (bIt->x >= eIt->x - 2 && bIt->x <= eIt->x + 2 &&
						bIt->y >= eIt->y - 1 && bIt->y <= eIt->y + 1) {
						gameMap::score += 1;
						eIt = enemyPlane::enemies.erase(eIt);
						hit = true;
						break;
					}
				}
				if (hit) {
					bIt = Bullet::bullets.erase(bIt);
				}
				else {
					++bIt;
				}
			}
			else {
				++bIt;
			}
		}

		//�л��������ײ���
		for (auto it = enemyPlane::enemies.begin(); it != enemyPlane::enemies.end(); ++it) {
			if (it->x - 2 <= player.x + 2 && it->x >= player.x - 2 &&
				it->y - 1 <= player.y + 1 && it->y >= player.y - 1) {
				gameMap::score -= 1;
				if (gameMap::score < 0) {
					gameMap::gameOver = true;
				}
				it = enemyPlane::enemies.erase(it);
				if (it == enemyPlane::enemies.end()) break;
			}
		}

		//�ӵ��������ײ���
		for (auto it = Bullet::bullets.begin(); it != Bullet::bullets.end();) {
			if (!it->isPlayer) {
				if (it->x >= player.x - 2 && it->x <= player.x + 2 &&
					it->y >= player.y - 1 && it->y <= player.y + 1) {
					gameMap::score -= 1;
					if (gameMap::score < 0) {
						gameMap::gameOver = true;
					}
					it = Bullet::bullets.erase(it);
				}
				else {
					++it;
				}
			}
			else {
				++it;
			}
		}

		// ���ƻ���
		ConsoleOperator::clearScreen();
		gameMap::drawBorder();
		gameMap::printScore();
		player.draw();
		Bullet::drawAll();
		enemyPlane::drawAll();
	}

	// ��Ϸ������ʾ
	ConsoleOperator::clearScreen();
	ConsoleOperator::gotoXY(20, 10);
	std::cout << "GAME OVER!";
	std::this_thread::sleep_for(std::chrono::seconds(3)); // �ӳ�3��
	return 0;
}