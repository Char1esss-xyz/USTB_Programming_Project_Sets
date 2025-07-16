#include "consoleOperator.h"

//���ع��
void ConsoleOperator::hideCursor() {
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = FALSE;
    cci.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

//�ƶ����
void ConsoleOperator::gotoXY(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


//����ˢ��
void ConsoleOperator::clearScreen() {
    system("cls");
}