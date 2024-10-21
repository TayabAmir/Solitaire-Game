#pragma once
#include <iostream>
#include<string>
#include<windows.h>
#include<unordered_map>
using namespace std;

void setTextColor(int textColor) {
	// Get the current console screen buffer info to retrieve the background color
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	// Mask the background color and apply the new text color
	int currentAttributes = csbi.wAttributes;
	int newAttributes = (currentAttributes & 0xF0) | textColor;  // Preserve the background (0xF0 masks the background)

	// Set the new console attributes (text color + existing background color)
	SetConsoleTextAttribute(hConsole, newAttributes);
}

class Card
{
	unordered_map<int, string> mapSuits = {
	{1, "Hearts"}, {2, "Spades"}, {3, "Diamonds"}, {4, "Clubs"}
	};
	unordered_map<int, string> mapRanks = {
	{1, "A"}, {11, "Jack"}, {12, "Queen"}, {13, "King"}
	};

public:
	string suit;
	int rank;
	bool isFaceUp;

	Card() : suit("Unknown"), rank(0), isFaceUp(false) {}
	Card(int suit, int rank) {
		this->suit = mapSuits[suit];
		this->rank = rank;
		this->isFaceUp = false;
	}

	void display() const {
		string s = (mapRanks.find(rank) != mapRanks.end()) ? mapRanks.at(rank) : to_string(rank);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		
        if (suit == "Hearts" || suit == "Diamonds") {
			setTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);  // Set red text without changing background
		}
		else {
			setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		cout << rank << "[" << suit << "]";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
};