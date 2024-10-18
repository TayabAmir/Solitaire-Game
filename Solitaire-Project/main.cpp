#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include "Stack.h"
#include "Queue.h"
#include "Card.cpp"
#include "LinkedList.cpp"
#include "Node.cpp"
using namespace std;

int main() {
	vector<Card> cards;

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 13; j++) {
			cards.push_back(Card(i, j));
		}
	}

	srand(time(0));
	for (int lastIndex = cards.size() - 1; lastIndex > 0; lastIndex--) {
		int j = (int)rand() % lastIndex;
		swap(cards[lastIndex], cards[j]);
	}

	for (auto& card : cards) {
		card.display();
	}
}