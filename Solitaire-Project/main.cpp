#include<iostream> 
#include<vector> 
#include<cstdlib> 
#include<ctime> 
#include <algorithm>
#include<conio.h> 
#include<windows.h> 
#include<unordered_map> 
#include "Stack.h"
#include "Queue.h"
#include "Card.h" 
#include "LinkedList.h" 
#include "Node.h" 
#include "ConsoleUtility.h"
#include "Move.h"

using namespace std;

class SolitaireGame {
private:
	LinkedList<Card> tableaus[7];
	Stack<Card> foundations[4];
	Queue<Card> stockPile, wastePile;
	Stack<Move> undoStack, redoStack;

	void gotoxy(int x, int y) {
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	void displayGame() {
		system("cls");

		// StockPile and WastePile at top left
		gotoxy(2, 1);
		ConsoleUtility::setConsoleColors(2, 15);
		cout << "Stock Pile: ";
		ConsoleUtility::setConsoleColors(0, 15);
		ConsoleUtility::setConsoleColors(1, 15);
		if (stockPile.empty()) {
			cout << stockPile.size() << " cards";
		}
		else {
			cout << stockPile.size() << " Cards";
		}
		ConsoleUtility::setConsoleColors(0, 15);


		// Position of WastePile below StockPile
		gotoxy(2, 3);
		ConsoleUtility::setConsoleColors(2, 15);
		cout << "Waste Pile: ";
		ConsoleUtility::setConsoleColors(0, 15);
		if (wastePile.empty()) {
			ConsoleUtility::setConsoleColors(1, 15);
			cout << "Empty";
			ConsoleUtility::setConsoleColors(0, 15);
		}
		else {
			Queue<Card> temp = wastePile;
			while (temp.size() > 1) {
				temp.dequeue();
			}
			temp.peek().display(); // Showing the last card in queue
		}

		// Foundations at top right
		gotoxy(70, 1);
		ConsoleUtility::setConsoleColors(2, 15);
		cout << "Foundations";
		ConsoleUtility::setConsoleColors(0, 15);
		for (int i = 0; i < 4; i++) {
			gotoxy(55 + i * 15, 3);
			if (foundations[i].empty()) {
				ConsoleUtility::setConsoleColors(1, 15);
				cout << "[Empty]";
				ConsoleUtility::setConsoleColors(0, 15);
			}
			else {
				foundations[i].top().display();
			}
		}

		int x = 5;
		int y = 6;
		for (int i = 0; i < 7; i++) {
			gotoxy(x + i * 15, y);

			ConsoleUtility::setConsoleColors(2, 15);
			cout << "Tableau " << i + 1;
			ConsoleUtility::setConsoleColors(0, 15);
			printTableau(tableaus[i].head, x + i * 15, y + 2);
		}
	}

	void printTableau(Node<Card>* temp, int x, int y) {
		Node<Card>* c = temp;
		while (c)
		{
			gotoxy(x, y);
			if (c->val.isFaceUp)
			{
				c->val.display();
			}
			else
			{
				ConsoleUtility::setConsoleColors(1, 15);
				cout << "Card Down" << endl;
				ConsoleUtility::setConsoleColors(0, 15);
			}
			c = c->next;
			y++;
		}
	}

	bool isValidMove(Card& c, Card& topCard) {
		// Conditions for valid move i.e different colors and one lower rank
		if ((c.rank == topCard.rank - 1) &&
			(((c.suit == "Hearts" || c.suit == "Diamonds") && (topCard.suit == "Clubs" || topCard.suit == "Spades"))
				||
				((c.suit == "Clubs" || c.suit == "Spades") && (topCard.suit == "Hearts" || topCard.suit == "Diamonds"))))
			return true;
		return false;
	}

	bool isValidMoveForF(Card& c, Card& topCard) {
		return c.rank - 1 == topCard.rank && c.suit == topCard.suit; // For card to be moved in foundation it should have same suit as the card in the foundation and 1 rank higher
	}

	void drawCard() {
		// If stock pile is empty, put all cards from wastepile to stockpile
		if (stockPile.empty()) {
			while (!wastePile.empty()) {
				Card card = wastePile.peek();
				card.isFaceUp = false;
				stockPile.enqueue(card);
				wastePile.dequeue();
			}
		}
		// If stockpile not empty take card from it
		else {
			wastePile.enqueue(stockPile.peek());
			stockPile.dequeue();
			wastePile.peek().isFaceUp = true;
		}
	}

	// winning condition is if all the foundations have 13 cards
	bool win() {
		for (int i = 0; i < 4; i++) {
			if (foundations[i].size() != 13) {
				return false;
			}
		}
		return true;
	}

	void moveCardBetweenTableaus(LinkedList<Card>& from, LinkedList<Card>& to, int noOfCards) {
		Node<Card>* head = from.head;
		int s = from.size();
		if (s == noOfCards) {
			// If the size of linked list from where we are transferring cards = no of cards we have to transfer, we simply connect the tail pointer of source list to the head of destination list
			if (to.tail) {
				to.tail->next = head;
			}
			else {
				to.head = head;
			}
			to.tail = from.tail;
			from.head = from.tail = NULL;
		}
		else {
			// Else changing the pointers accordingly
			for (int i = 0; i < s - noOfCards - 1; i++) {
				head = head->next;
			}
			if (to.tail) {
				to.tail->next = head->next;
			}
			else {
				to.head = head->next;
			}
			to.tail = from.tail;
			head->next = NULL;
			from.tail = head;
			from.tail->val.isFaceUp = true;
		}
	}

	void moveCardFromWtoF(Queue<Card>& w, Stack<Card>& f) {
		Queue<Card> temp;
		while (w.size() > 1) {
			temp.enqueue(w.peek());
			w.dequeue();
		}
		Card c = w.peek();
		w.dequeue();
		if ((c.rank == 1 && f.empty()) || (!f.empty() && isValidMoveForF(c, f.top()))) {
			c.isFaceUp = true;
			f.push(c);
		}
		else {
			cout << "Invalid Move" << endl;
			w.enqueue(c);
		}
		while (!temp.empty()) {
			w.enqueue(temp.peek());
			temp.dequeue();
		}
	}

	void moveCarFromTtoF(LinkedList<Card>& t, Stack<Card>& f) {
		Card c = t.tail->val;
		f.push(c);
		t.deleteFromEnd();
		if (t.tail)
			t.tail->val.isFaceUp = true;
	}

	void moveCardFromWtoT(Queue<Card>& w, LinkedList<Card>& t) {

		Queue<Card> temp;
		while (w.size() > 1) {
			temp.enqueue(w.peek());
			w.dequeue();
		}
		bool valid = true;
		Card c = w.peek();
		w.dequeue();
		if ((t.isEmpty() && c.rank == 13) || (!t.isEmpty() && isValidMove(c, t.tail->val))) {
			c.isFaceUp = true;
			t.insertAtEnd(c);
		}
		else {
			cout << "Invalid Move";
			valid = false;
		}
		while (!temp.empty()) {
			w.enqueue(temp.peek());
			temp.dequeue();
		}
		if (!valid)
			w.enqueue(c);
	}
	int noofFaceupCards(LinkedList<Card>& ll) {
		Node<Card>* temp = ll.head;
		int s = 0;
		while (temp)
		{
			if (temp->val.isFaceUp)
				s++;
			temp = temp->next;
		}
		return s;
	}

public:
	SolitaireGame() {
		// Making deck of 52 Cards

		vector<Card> cards;
		for (int i = 1; i <= 4; i++) {
			for (int j = 1; j <= 13; j++) {
				cards.push_back(Card(i, j));
			}
		}

		// Shuffling the made cards using Fisher-Yates shuffling algo
		srand(time(0));
		for (int lastIndex = cards.size() - 1; lastIndex > 0; lastIndex--) {
			int j = (int)rand() % lastIndex;
			swap(cards[lastIndex], cards[j]);
		}

		// Filling Tableaus with Cards
		int cardIdx = 0;
		for (int tableauIdx = 0; tableauIdx < 7; tableauIdx++) {
			for (int card = 1; card <= tableauIdx + 1; card++) {
				tableaus[tableauIdx].insertAtEnd(cards[cardIdx]);
				if (card == tableauIdx + 1)
					tableaus[tableauIdx].tail->val.isFaceUp = true;
				cardIdx++;
			}
		}

		// Filling StockPile with Cards
		while (cardIdx < cards.size()) {
			stockPile.enqueue(cards[cardIdx++]);
		}
	}
	void moveCards(int from, int fromIdx, int to, int toIdx, int& noOfCards, bool& lastCardFace) {
		if (from == 1 && fromIdx >= 1 && fromIdx <= 7) {
			// From Tableau to Tableau
			if (to == 1 && toIdx >= 1 && toIdx <= 7) {
				if (noOfCards == -1) {
					cout << "Enter Number of Cards you want to move: ";
					cin >> noOfCards;

					if (tableaus[fromIdx - 1].size() < noOfCards) {
						cout << "There are not enough cards in Tableau " << fromIdx << "." << endl;
						return;
					}
					if (noOfCards > noofFaceupCards(tableaus[fromIdx - 1])) {
						cout << "Not enough face up cards." << endl;
						return;
					}
				}

				Node<Card>* head = tableaus[fromIdx - 1].head;

				if (tableaus[fromIdx - 1].size() != noOfCards) {
					for (int i = 0; i < tableaus[fromIdx - 1].size() - noOfCards - 1; i++) {
						head = head->next;
					}
				}

				Node<Card>* x = head;
				lastCardFace = head->val.isFaceUp;
				if (tableaus[fromIdx - 1].size() != noOfCards) {
					x = x->next;
				}
				// If Tableau is empty and the card to be move from is king
				if (tableaus[toIdx - 1].isEmpty() && x->val.rank == 13) {
					Node<Card>* temp = head->next;
					while (temp) {
						tableaus[toIdx - 1].insertAtEnd(temp->val);
						temp = temp->next;
					}
					head->next = NULL;
					tableaus[fromIdx - 1].tail = head;
					if(tableaus[fromIdx - 1].tail)
						tableaus[fromIdx - 1].tail->val.isFaceUp = true;
					return;
				}
				else if (tableaus[toIdx - 1].isEmpty() && x->val.rank != 13) {
					return;
				}


				if (!isValidMove(x->val, tableaus[toIdx - 1].tail->val)) {
					cout << "Invalid Move." << endl;
					return;
				}

				moveCardBetweenTableaus(tableaus[fromIdx - 1], tableaus[toIdx - 1], noOfCards);
			}
			// From Tableau to Foundation
			else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
				Card c = tableaus[fromIdx - 1].tail->val;
				if (tableaus[fromIdx - 1].size() >= 1) {
					if (foundations[toIdx - 1].empty()) {
						if (c.rank == 1) {
							moveCarFromTtoF(tableaus[fromIdx - 1], foundations[toIdx - 1]);
						}
						else {
							cout << "Only an ace card can be moved to empty foundation.";
						}
					}
					else if (isValidMoveForF(c, foundations[toIdx - 1].top())) {
						moveCarFromTtoF(tableaus[fromIdx - 1], foundations[toIdx - 1]);
					}
					else {
						cout << "Cannot move this card to foundation";
					}
				}
			}
			else {
				cout << "Invalid destination!" << endl;
			}
		}
		else if (from == 2) {
			// From WastePile to Tableau
			if (to == 1 && toIdx >= 1 && toIdx <= 7) {
				if (!wastePile.empty())
					moveCardFromWtoT(wastePile, tableaus[toIdx - 1]);
				else
					cout << "Waste Pile is empty.";
			}
			// From WastePile to Foundation
			else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
				if (!wastePile.empty())
					moveCardFromWtoF(wastePile, foundations[toIdx - 1]);
				else
					cout << "Waste Pile is empty.";
			}
			else {
				cout << "Invalid destination!" << endl;
			}
		}
		else {
			cout << "Invalid source!" << endl;
		}
	}

	void moveFromWToS() {
		Queue<Card> temp;
		while (wastePile.size() > 1) {
			temp.enqueue(wastePile.peek());
			wastePile.dequeue();
		}
		Card c = wastePile.peek();
		wastePile.dequeue();
		while (!temp.empty()) {
			wastePile.enqueue(temp.peek());
			temp.dequeue();
		}
		stockPile.enqueue(c);
	}

	void play() {
		string cmd;
		while (true) {
			ConsoleUtility::setConsoleColors(0, 15);
			displayGame();

			gotoxy(2, 25);
			cout << "Enter command (move, draw, quit): ";
			cin >> cmd;

			int from = -1, fromIdx = -1, to = -1, toIdx = -1, noOfCards = -1;
			bool lastCardFace = false;
			if (cmd == "move") {
				cout << "Enter source (1 for Tableau, 2 for Waste Pile): ";
				cin >> from;
				if (from == 1) {
					cout << "Enter the Tableau Number: ";
					cin >> fromIdx;
				}
				cout << "Enter destination (1 for Tableau, 2 for Foundation): ";
				cin >> to;
				cout << "Enter the Number: ";
				cin >> toIdx;

				moveCards(from, fromIdx, to, toIdx, noOfCards, lastCardFace);
				char ch = _getch();
			}
			else if (cmd == "draw") {
				drawCard();
			}
			else if (cmd == "undo") {

				if (undoStack.empty()) {
					cout << "No move to undo :)";
				}
				else {
					Move m = undoStack.top();
					undoStack.pop();
					redoStack.push(m);

					if (m.cmd == "draw") {
						moveFromWToS();
					}
					else {
						if (m.from == 2 && m.fromIdx == -1) {
							if (m.to == 1) {
								Card c = tableaus[m.toIdx - 1].tail->val;
								tableaus[m.toIdx - 1].deleteFromEnd();
								wastePile.enqueue(c);
							}
							else {
								Card c = foundations[m.toIdx - 1].top();
								foundations[m.toIdx - 1].pop();
								wastePile.enqueue(c);
							}
						}
						if (m.from == 1) {
							if (m.to == 1) {
								int cards = m.noOfCards;
								Stack<Card> temp;
								while (cards--) {
									temp.push(tableaus[m.toIdx - 1].tail->val);
									tableaus[m.toIdx - 1].deleteFromEnd();
								}
								if(tableaus[m.fromIdx - 1].tail)
									tableaus[m.fromIdx - 1].tail->val.isFaceUp = m.face;
								while (!temp.empty()) {
									Card c = temp.top(); temp.pop();
									tableaus[m.fromIdx - 1].insertAtEnd(c);
								}
							}
							else {
								Card c = foundations[m.toIdx - 1].top();
								foundations[m.toIdx - 1].pop();
								tableaus[m.fromIdx - 1].tail->val.isFaceUp = false;
								tableaus[m.fromIdx - 1].insertAtEnd(c);
							}
						}
					}
				}
			}
			else if (cmd == "redo") {
				if (redoStack.empty()) {
					cout << "No Move to Redo :(";
				}
				else {
					Move m = redoStack.top();
					undoStack.push(m);
					redoStack.pop();
					if (m.cmd == "draw") {
						drawCard();
					}
					else if (m.cmd == "move") {
						moveCards(m.from, m.fromIdx, m.to, m.toIdx, m.noOfCards, m.face);
					}
				}
			}
			else if (cmd == "quit") {
				break;
			}
			else {
				cout << "Invalid command!" << endl;
				char ch = _getch();
			}
			if (win()) {
				displayGame();
				cout << endl << "Congratulations! You've won the game!" << endl;
				break;
				char ch = _getch();
			}
			if (cmd == "move") {
				Move m(cmd, from, fromIdx, to, toIdx, noOfCards, lastCardFace);
				undoStack.push(m);
			}
			else if (cmd == "draw") {
				Move m(cmd);
				undoStack.push(m);
			}
		}
	}
};


int main() {

	SolitaireGame game;
	game.play();
	return 0;
}