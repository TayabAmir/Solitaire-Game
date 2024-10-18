#pragma once
#include <iostream>
#include<string>
#include<unordered_map>
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

	void display() {
		string s = (mapRanks.find(rank) != mapRanks.end()) ? mapRanks[rank] : to_string(rank);
		cout << s << " Of " << suit << endl;
	}
};