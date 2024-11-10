#pragma once
#include <iostream>
#include <windows.h>
using namespace std;

class ConsoleUtility
{
public:
    static void setConsoleColors(int textColor, int backgroundColor) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (backgroundColor << 4) | textColor);
    }
    static void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

};
