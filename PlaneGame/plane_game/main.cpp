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
	// 隐藏光标，清屏
	ConsoleOperator::hideCursor();
	ConsoleOperator::clearScreen();

	// 初始化全局游戏状态
	gameMap::score = 0;
	gameMap::gameOver = false;

	while (!gameMap::gameOver) {
		Sleep(100);

		player.move();

		// 更新
		Bullet::updateAll();
		enemyPlane::updateAll();


		if (rand() % 10 == 0) {
			enemyPlane::spawn();
		}

		//子弹和敌机碰撞检测
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

		//敌机和玩家碰撞检测
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

		//子弹和玩家碰撞检测
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

		// 绘制画面
		ConsoleOperator::clearScreen();
		gameMap::drawBorder();
		gameMap::printScore();
		player.draw();
		Bullet::drawAll();
		enemyPlane::drawAll();
	}

	// 游戏结束提示
	ConsoleOperator::clearScreen();
	ConsoleOperator::gotoXY(20, 10);
	std::cout << "GAME OVER!";
	std::this_thread::sleep_for(std::chrono::seconds(3)); // 延迟3秒
	return 0;
}