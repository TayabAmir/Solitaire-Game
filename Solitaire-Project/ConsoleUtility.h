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
};
