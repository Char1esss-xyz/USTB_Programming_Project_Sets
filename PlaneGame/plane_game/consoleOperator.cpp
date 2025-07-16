#include "consoleOperator.h"

//隐藏光标
void ConsoleOperator::hideCursor() {
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = FALSE;
    cci.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

//移动光标
void ConsoleOperator::gotoXY(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


//清屏刷新
void ConsoleOperator::clearScreen() {
    system("cls");
}