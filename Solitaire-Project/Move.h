#pragma once
#include <iostream>
#include<string>
using namespace std;

class Move
{
public:
	char cmd;
	int from, fromIdx, to, toIdx;
	int noOfCards;
	bool face;

	Move() {
		cmd = '.';
		from = fromIdx = to = toIdx = noOfCards = -1;
		face = false;
	}

	Move(char cmd) {
		this->cmd = cmd;
		from = fromIdx = to = toIdx = noOfCards = -1;
		face = false;
	}

	Move(char cmd, int from, int fromIdx, int to, int toIdx, int noOfCards, bool face) {
		this->cmd = cmd;
		this->from = from;
		this->fromIdx = fromIdx;
		this->to = to;
		this->toIdx = toIdx;
		this->noOfCards = noOfCards;
		this->face = face;
	}
};