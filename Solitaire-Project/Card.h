﻿#pragma once
#include <iostream>
#include<string>
#include<windows.h>
#include<unordered_map>
#include "ConsoleUtility.h"
using namespace std;


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
		string mappedRank = (mapRanks.find(rank) != mapRanks.end()) ? mapRanks.at(rank) : to_string(rank);
		if (suit == "Hearts" || suit == "Diamonds") {
				ConsoleUtility::setConsoleColors(4, 15);
		}
		cout << mappedRank << "[" << suit << "]";
		ConsoleUtility::setConsoleColors(0, 15);

	}
};