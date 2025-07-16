#include "gameMap.h"
#include "consoleOperator.h"
#include <iostream>

int gameMap::score = 0;
bool gameMap::gameOver = false;

//绘制边界
void gameMap::drawBorder() {
    for (int x = 0; x < width; ++x) {
        ConsoleOperator::gotoXY(x, 0); std::cout << "#";
        ConsoleOperator::gotoXY(x, height - 1); std::cout << "#";
    }
    for (int y = 1; y < height - 1; ++y) {
        ConsoleOperator::gotoXY(0, y); std::cout << "#";
        ConsoleOperator::gotoXY(width - 1, y); std::cout << "#";
    }
}

//打印分数
void gameMap::printScore() {
    ConsoleOperator::gotoXY(1, 0);
    std::cout << "Score: " << score;
}