#include<iostream> 
#include<vector> 
#include<cstdlib> 
#include<ctime> 
#include <algorithm>
#include<conio.h> 
#include<windows.h> 
#include<unordered_map>
#include <chrono>
#include <thread>
#include <atomic>
#include "Stack.h"
#include "Queue.h"
#include "Card.h" 
#include "LinkedList.h" 
#include "Node.h" 
#include "ConsoleUtility.h"
#include "Move.h"
using namespace std;

atomic<bool> keepRunning(true);

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void startTimer() {
	auto start = std::chrono::steady_clock::now();
	while (keepRunning) {
		auto now = chrono::steady_clock::now();
		auto elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		COORD originalCursorPosition = csbi.dwCursorPosition;

		gotoxy(85, 25);
		cout << "Time passed: " << elapsed << " seconds    " << flush;

		SetConsoleCursorPosition(hConsole, originalCursorPosition);

		this_thread::sleep_for(chrono::seconds(1));
	}
}


class SolitaireGame {
private:
	LinkedList<Card> tableaus[7];
	Stack<Card> foundations[4];
	Queue<Card> stockPile, wastePile;
	Stack<Move> undoStack, redoStack;
	int moves;

	void displayGame() {
		system("cls");

		gotoxy(2, 1);
		ConsoleUtility::setConsoleColors(2, 15);
		cout << "Stock Pile: ";
		ConsoleUtility::setConsoleColors(0, 15);
		ConsoleUtility::setConsoleColors(1, 15);
		if (stockPile.empty()) {
			cout << "Empty";
		}
		else {
			cout << stockPile.size() << " Cards";
		}
		ConsoleUtility::setConsoleColors(0, 15);


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
			temp.peek().display();
		}

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
		gotoxy(85, 26);
		cout << "Moves: " << moves;

		gotoxy(85, 27);
		cout << undoStack.size();

	}

	void printTableau(Node<Card>* temp, int x, int y) {
		Node<Card>* c = temp;
		if (!c) {
			gotoxy(x, y);
			cout << "[Empty]";
		}
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
		if ((c.rank == topCard.rank - 1) &&
			(((c.suit == "Hearts" || c.suit == "Diamonds") && (topCard.suit == "Clubs" || topCard.suit == "Spades"))
				||
				((c.suit == "Clubs" || c.suit == "Spades") && (topCard.suit == "Hearts" || topCard.suit == "Diamonds"))))
			return true;
		return false;
	}

	bool isValidMoveForF(Card& c, Card& topCard) {
		return c.rank - 1 == topCard.rank && c.suit == topCard.suit;
	}

	void drawCard() {
		if (stockPile.empty()) {
			while (!wastePile.empty()) {
				Card card = wastePile.peek();
				card.isFaceUp = false;
				stockPile.enqueue(card);
				wastePile.dequeue();
			}
		}
		else {
			wastePile.enqueue(stockPile.peek());
			stockPile.dequeue();
			wastePile.peek().isFaceUp = true;
		}
	}

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

	bool moveCardFromWtoF(Queue<Card>& w, Stack<Card>& f) {
		Queue<Card> temp;
		while (w.size() > 1) {
			temp.enqueue(w.peek());
			w.dequeue();
		}
		bool t = true;
		Card c = w.peek();
		w.dequeue();
		if ((c.rank == 1 && f.empty()) || (!f.empty() && isValidMoveForF(c, f.top()))) {
			c.isFaceUp = true;
			f.push(c);
		}
		else {
			cout << "Invalid Move" << endl;
			t = false;
		}
		while (!temp.empty()) {
			w.enqueue(temp.peek());
			temp.dequeue();
		}
		w.enqueue(c);
		return t;
	}

	void moveCarFromTtoF(LinkedList<Card>& t, Stack<Card>& f) {
		Card c = t.tail->val;
		f.push(c);
		t.deleteFromEnd();
		if (t.tail)
			t.tail->val.isFaceUp = true;
	}

	bool moveCardFromWtoT(Queue<Card>& w, LinkedList<Card>& t) {
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
		return valid;
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
	SolitaireGame(int shuffle) {
		vector<Card> cards;

		if (shuffle == 0) {
			cards = {
				Card(1, 1),
				Card(1, 2),Card(3, 1),
				Card(3, 13), Card(1, 3), Card(3, 2),
				Card(1, 4), Card(3, 3), Card(1, 5), Card(2, 1),
				Card(1, 6), Card(3, 4), Card(1, 7), Card(3, 5), Card(1, 8),
				Card(1, 9), Card(3, 6), Card(1, 10), Card(3, 7), Card(1, 11), Card(3, 8),
				Card(1, 12), Card(3, 9), Card(1, 13), Card(3, 10), Card(2, 12), Card(3, 11), Card(3, 12),
				Card(4, 1), Card(2, 2), Card(2, 3), Card(2, 4), Card(2, 5), Card(2, 6), Card(2, 7), Card(2, 8), Card(2, 9), Card(2, 10),
				Card(2, 11), Card(2, 13), Card(4, 2), Card(4, 3), Card(4, 4), Card(4, 5), Card(4, 6), Card(4, 7), Card(4, 8), Card(4, 9),
				Card(4, 10), Card(4, 11), Card(4, 12), Card(4, 13)
			};

		}
		else if (shuffle == 1) {
			cards = {
				Card(1, 1), Card(3, 1), Card(1, 2), Card(4, 1), Card(1, 3), Card(3, 2), Card(2, 1), Card(1, 4), Card(3, 3), Card(1, 5),
				Card(1, 6), Card(3, 4), Card(1, 7), Card(3, 5), Card(1, 8), Card(1, 9), Card(3, 6), Card(1, 10), Card(3, 7), Card(1, 11),
				Card(3, 8), Card(1, 12), Card(3, 9), Card(1, 13), Card(3, 10), Card(2, 12), Card(3, 11), Card(3, 12), Card(3,13), Card(2, 2),
				Card(2, 3), Card(2, 4), Card(2, 5), Card(2, 6), Card(2, 7), Card(2, 8), Card(2, 9), Card(2, 10), Card(2, 11), Card(2, 13),
				Card(4, 2), Card(4, 3), Card(4, 4), Card(4, 5), Card(4, 6), Card(4, 7), Card(4, 8), Card(4, 9), Card(4, 10), Card(4, 11),
				Card(4, 12), Card(4, 13)
			};
		}
		else if (shuffle == 2) {
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
		}
		else if (shuffle == 3) {
			cards = {
				Card(1, 5), Card(1, 6), Card(3, 4), Card(3, 5), Card(1, 12), Card(3, 9), Card(3, 1), Card(1, 7), Card(1, 11), Card(3, 3),
				Card(1, 1), Card(3, 8), Card(1, 8), Card(3, 13), Card(1, 4), Card(4, 1), Card(2, 12), Card(3, 6), Card(3, 7), Card(1, 9),
				Card(3, 11), Card(2, 1), Card(3, 2), Card(3, 12), Card(1, 13), Card(1, 10), Card(3, 10), Card(4, 4), Card(2, 2), Card(2, 3),
				Card(2, 4), Card(2, 5), Card(2, 6), Card(2, 7), Card(2, 8), Card(2, 9), Card(2, 10), Card(2, 11), Card(2, 13), Card(4, 2),
				Card(4, 3), Card(4, 5), Card(4, 6), Card(4, 7), Card(4, 8), Card(4, 9), Card(4, 10), Card(4, 11), Card(4, 12), Card(4, 13),
				Card(1, 2), Card(1, 3)
			};
		}

		int cardIdx = 0;
		for (int tableauIdx = 0; tableauIdx < 7; tableauIdx++) {
			for (int card = 1; card <= tableauIdx + 1; card++) {
				tableaus[tableauIdx].insertAtEnd(cards[cardIdx]);
				if (card == tableauIdx + 1)
					tableaus[tableauIdx].tail->val.isFaceUp = true;
				cardIdx++;
			}
		}

		while (cardIdx < cards.size()) {
			stockPile.enqueue(cards[cardIdx++]);
		}
		moves = 0;
	}

	bool moveCards(int from, int fromIdx, int to, int toIdx, int& noOfCards, bool& lastCardFace) {
		if (from == 1 && fromIdx >= 1 && fromIdx <= 7) {
			if (to == 1 && toIdx >= 1 && toIdx <= 7) {
				if (noOfCards == -1) {
					cout << "Enter Number of Cards you want to move: ";
					cin >> noOfCards;

					if (tableaus[fromIdx - 1].size() < noOfCards) {
						cout << "There are not enough cards in Tableau " << fromIdx << "." << endl;
						return false;
					}
					if (noOfCards > noofFaceupCards(tableaus[fromIdx - 1])) {
						cout << "Not enough face up cards." << endl;
						return false;
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

				if (tableaus[toIdx - 1].isEmpty() && x->val.rank == 13) {
					Node<Card>* temp = head->next;
					while (temp) {
						tableaus[toIdx - 1].insertAtEnd(temp->val);
						temp = temp->next;
					}
					head->next = NULL;
					tableaus[fromIdx - 1].tail = head;
					if (tableaus[fromIdx - 1].tail)
						tableaus[fromIdx - 1].tail->val.isFaceUp = true;
					return true;
				}
				else if (tableaus[toIdx - 1].isEmpty() && x->val.rank != 13) {
					return false;
				}

				if (!isValidMove(x->val, tableaus[toIdx - 1].tail->val)) {
					cout << "Invalid Move." << endl;
					return false;
				}

				moveCardBetweenTableaus(tableaus[fromIdx - 1], tableaus[toIdx - 1], noOfCards);
			}
			else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
				Card c = tableaus[fromIdx - 1].tail->val;
				if (tableaus[fromIdx - 1].size() >= 1) {
					if (foundations[toIdx - 1].empty()) {
						if (c.rank == 1) {
							moveCarFromTtoF(tableaus[fromIdx - 1], foundations[toIdx - 1]);
						}
						else {
							cout << "Only an ace card can be moved to empty foundation.";
							return false;
						}
					}
					else if (isValidMoveForF(c, foundations[toIdx - 1].top())) {
						moveCarFromTtoF(tableaus[fromIdx - 1], foundations[toIdx - 1]);
					}
					else {
						cout << "Cannot move this card to foundation";
						return false;
					}
				}
			}
			else {
				cout << "Invalid destination!" << endl;
				return false;
			}
		}
		else if (from == 2) {
			if (to == 1 && toIdx >= 1 && toIdx <= 7) {
				if (!wastePile.empty())
					return moveCardFromWtoT(wastePile, tableaus[toIdx - 1]);
				else {
					cout << "Waste Pile is empty.";
					return false;
				}
			}
			else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
				if (!wastePile.empty())
					return moveCardFromWtoF(wastePile, foundations[toIdx - 1]);
				else {
					cout << "Waste Pile is empty.";
					return false;
				}
			}
			else {
				cout << "Invalid destination!" << endl;
				return false;
			}
		}
		else {
			cout << "Invalid source!" << endl;
			return false;
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
			bool valid = false;
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
				valid = moveCards(from, fromIdx, to, toIdx, noOfCards, lastCardFace);
				char ch = _getch();
				moves++;
			}
			else if (cmd == "draw") {
				drawCard();
				moves++;
			}
			else if (cmd == "undo") {

				if (undoStack.empty()) {
					cout << "No move to undo :)";
				}
				else {
					moves++;
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
								if (tableaus[m.fromIdx - 1].tail)
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
					moves++;
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
			if (cmd == "move" && valid) {
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

void displayInstructions() {
	system("cls");
	gotoxy(10, 2);
	cout << "===============================" << endl;
	cout << "           Instructions         " << endl;
	cout << "===============================" << endl;
	cout << endl;
	gotoxy(10, 7);
	std::cout << "1. The goal is to move all cards to the foundation piles.";
	gotoxy(10, 9);
	std::cout << "2. Build up each foundation by suit, from Ace to King.";
	gotoxy(10, 11);
	std::cout << "3. You can move cards between tableau piles, building down by alternating colors.";
	gotoxy(10, 13);
	std::cout << "4. Use the stock pile to draw more cards if you get stuck.";
	gotoxy(10, 15);
	std::cout << "5. To win, all cards must be in the foundation piles.";

	gotoxy(10, 18);
	std::cout << "Press any key to return to the main menu.";
	char c = _getch();
}

void displayHeader() {
	ConsoleUtility::setConsoleColors(14, 0);
	gotoxy(20, 5);
	cout << "  SSSSS   OOOOO  L        III  TTTTT  AAAAA  RRRRR   EEEEE" << endl;
	gotoxy(20, 6);
	cout << " S        O   O  L         I     T   A     A R    R  E" << endl;
	gotoxy(20, 7);
	cout << "  SSS     O   O  L         I     T   AAAAAAA RRRRR   EEEE" << endl;
	gotoxy(20, 8);
	cout << "     S    O   O  L         I     T   A     A R  R    E" << endl;
	gotoxy(20, 9);
	cout << "  SSSSS   OOOOO  LLLLL    III    T   A     A R   RR  EEEEE" << endl;
	ConsoleUtility::setConsoleColors(15, 0);
}

void displayFrontPage() {
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	cout << "===============================" << endl;
	cout << "        Welcome to Solitaire!    " << endl;
	cout << "===============================" << endl;
	cout << endl;

	cout << "1. Start Game" << endl;
	cout << "2. Instructions" << endl;
	cout << "3. Quit" << endl;

	cout << endl;
	cout << "Please select an option (1-3): ";
}

void displayShuffleOptions() {
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	cout << "===============================" << endl;
	cout << "    Select Shuffle Difficulty   " << endl;
	cout << "===============================" << endl;
	cout << endl;

	cout << "1. Easy Shuffle" << endl;
	cout << "2. Medium Shuffle" << endl;
	cout << "3. Hard Shuffle" << endl;

	cout << endl;
	cout << "Please select a shuffle difficulty (1-3): ";
}


int main() {

	while (true) {
		displayFrontPage();

		char choice = _getch();

		if (choice == '1') {
			displayShuffleOptions();
			char c = _getch();
			int shfl = c - '0';

			thread timerThread(startTimer);
			if (shfl == 0) {
				SolitaireGame game(0);
				game.play();
			}
			else if (shfl == 1) {
				SolitaireGame game(1);
				game.play();
			}
			else if (shfl == 2) {
				SolitaireGame game(2);
				game.play();
			}
			else if (shfl == 3) {
				SolitaireGame game(3);
				game.play();
			}

			keepRunning = false;
			timerThread.join();
		}
		else if (choice == '2') {
			displayInstructions();
		}
		else if (choice == '3') {
			system("cls");
			cout << "Quitting game. Goodbye!" << std::endl;
			break;
		}
		else {
			gotoxy(24, 25);
			cout << "Invalid option. Please try again.";
			Sleep(1000);
		}
	}
	return 0;
}