#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "ConsoleUtility.h"
#include "Node.h"
#include "Stack.h"
#include "Card.h"
#include "Queue.h"
#include "LinkedList.h"
using namespace std;

class UI
{
public:
	static void displayInstructions() {
		system("cls");
		ConsoleUtility::setConsoleColors(4, 15);
		ConsoleUtility::gotoxy(50, 0);
		cout << "===============================" << endl;
		ConsoleUtility::gotoxy(50, 1);
		cout << "           Instructions         " << endl;
		ConsoleUtility::gotoxy(50, 2);
		cout << "===============================" << endl;

		ConsoleUtility::setConsoleColors(1, 15);
		ConsoleUtility::gotoxy(10, 5);
		cout << "1. The goal is to move all cards to the foundation piles.";
		ConsoleUtility::gotoxy(10, 7);
		cout << "2. Build up each foundation by suit, from Ace to King.";
		ConsoleUtility::gotoxy(10, 9);
		cout << "3. You can move cards between tableau piles, building down by alternating colors.";
		ConsoleUtility::gotoxy(10, 11);
		cout << "4. Use the stock pile to draw more cards if you get stuck.";
		ConsoleUtility::gotoxy(10, 13);
		cout << "5. To win, all cards must be in the foundation piles.";

		displayScore();
		ConsoleUtility::setConsoleColors(0, 15);
		ConsoleUtility::gotoxy(10, 39);
		cout << "Press any key to return to the main menu.";
		char c = _getch();

	}

	static void displayHeader() {
		int x = 50;
		ConsoleUtility::setConsoleColors(4, 15);
		ConsoleUtility::gotoxy(x, 0);
		cout << "===========================================";
		ConsoleUtility::gotoxy(x, 1);
		cout << "             K L O N D I K E";
		ConsoleUtility::gotoxy(x, 2);
		cout << "              S O L I T A I R E";
		ConsoleUtility::gotoxy(x, 3);
		cout << "===========================================";
		ConsoleUtility::setConsoleColors(0, 15);
	}

	static void displayFrontPage() {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		ConsoleUtility::setConsoleColors(4, 15);
		ConsoleUtility::gotoxy(50, 0);
		cout << "===============================";
		ConsoleUtility::gotoxy(50, 1);
		cout << "        Welcome to Solitaire!    ";
		ConsoleUtility::gotoxy(50, 2);
		cout << "===============================";
		ConsoleUtility::setConsoleColors(1, 15);


		ConsoleUtility::gotoxy(3, 6);
		cout << "1. Start Game" << endl;
		ConsoleUtility::gotoxy(3, 7);
		cout << "2. Instructions" << endl;
		ConsoleUtility::gotoxy(3, 8);
		cout << "3. Quit" << endl;

		ConsoleUtility::gotoxy(3, 10);
		ConsoleUtility::setConsoleColors(0, 15);
		cout << "Please select an option (1-3): ";
	}

	static void displayScore() {
		ConsoleUtility::setConsoleColors(4, 15);
		ConsoleUtility::gotoxy(50, 16);
		cout << "===============================";
		ConsoleUtility::gotoxy(50, 17);
		cout << "        Scoring Criteria    ";
		ConsoleUtility::gotoxy(50, 18);
		cout << "===============================";
		ConsoleUtility::setConsoleColors(1, 15);
		int x = 10;
		int y = 21;

		ConsoleUtility::gotoxy(x, y);
		cout << "-------------------------------------------------------------";
		ConsoleUtility::gotoxy(x, y + 1);
		cout << "| Move                              |          Points       |";
		ConsoleUtility::gotoxy(x, y + 2);
		cout << "-------------------------------------------------------------";

		ConsoleUtility::gotoxy(x, y + 3);
		cout << "| Waste to Tableau                  |            5          |";
		ConsoleUtility::gotoxy(x, y + 4);
		cout << "-------------------------------------------------------------";

		ConsoleUtility::gotoxy(x, y + 5);
		cout << "| Waste to Foundation               |            10         |";
		ConsoleUtility::gotoxy(x, y + 6);
		cout << "-------------------------------------------------------------";

		ConsoleUtility::gotoxy(x, y + 7);
		cout << "| Tableau to Foundation             |            10         |";
		ConsoleUtility::gotoxy(x, y + 8);
		cout << "-------------------------------------------------------------";

		ConsoleUtility::gotoxy(x, y + 9);
		cout << "| Turn over Tableau card            |            5          |";
		ConsoleUtility::gotoxy(x, y + 10);
		cout << "-------------------------------------------------------------";

		ConsoleUtility::gotoxy(x, y + 11);
		cout << "| Recycle waste                     |          -100         |";
		ConsoleUtility::gotoxy(x, y + 12);
		cout << "-------------------------------------------------------------";
	}

	static void displayShuffleOptions() {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		ConsoleUtility::setConsoleColors(4, 15);
		ConsoleUtility::gotoxy(50, 0);
		cout << "===============================" << endl;
		ConsoleUtility::gotoxy(50, 1);
		cout << "    Select Shuffle Difficulty   " << endl;
		ConsoleUtility::gotoxy(50, 2);
		cout << "===============================" << endl;
		cout << endl;
		ConsoleUtility::setConsoleColors(1, 15);

		ConsoleUtility::gotoxy(3, 6);
		cout << "1. Easy Shuffle" << endl;
		ConsoleUtility::gotoxy(3, 7);
		cout << "2. Medium Shuffle" << endl;
		ConsoleUtility::gotoxy(3, 8);
		cout << "3. Hard Shuffle" << endl;

		ConsoleUtility::setConsoleColors(0, 15);
		ConsoleUtility::gotoxy(3, 11);
		cout << "Please select a shuffle difficulty (1-3): ";
	}

	static void displayGame(LinkedList<Card> tableaus[], Stack<Card> foundations[], Queue<Card>& stockPile, Queue<Card>& wastePile, int& score, int& moves) {
		int y = 5;
		system("cls");
		displayHeader();
		ConsoleUtility::gotoxy(2, 1 + y);
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
		ConsoleUtility::setConsoleColors(0, 15 + y);


		ConsoleUtility::gotoxy(2, 3 + y);
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

		ConsoleUtility::gotoxy(104, 1 + y);
		ConsoleUtility::setConsoleColors(2, 15);
		cout << "Foundations";
		ConsoleUtility::setConsoleColors(0, 15);
		for (int i = 0; i < 4; i++) {
			ConsoleUtility::gotoxy(75 + i * 20, 3 + y);
			if (foundations[i].empty()) {
				ConsoleUtility::setConsoleColors(1, 15);
				cout << "[Empty]";
				ConsoleUtility::setConsoleColors(0, 15);
			}
			else {
				foundations[i].top().display();
			}
		}

		int x = 10;
		int ty = 8;
		for (int i = 0; i < 7; i++) {
			ConsoleUtility::gotoxy(x + i * 20, ty + y);

			ConsoleUtility::setConsoleColors(2, 15);
			cout << "Tableau " << i + 1;
			ConsoleUtility::setConsoleColors(0, 15);
			printTableau(tableaus[i].head, x + i * 20, ty + 2 + y);
		}
		ConsoleUtility::gotoxy(115, 2);
		ConsoleUtility::setConsoleColors(5, 15);
		cout << "Moves: " << moves;
		ConsoleUtility::gotoxy(115, 3);
		cout << "Score: " << score;
		ConsoleUtility::setConsoleColors(0, 15);

	}

	static void displayWinningScreen(int score, int moves) {
		system("cls");
		displayHeader();
		int x = 60, y = 10;
		ConsoleUtility::gotoxy(x, y);
		ConsoleUtility::setConsoleColors(1, 15);
		cout << "Congratulations!" << endl;

		ConsoleUtility::gotoxy(x, y+1);
		cout << "You've won the game!" << endl;

		ConsoleUtility::gotoxy(x, y+3);
		cout << "Score: " << score << endl;

		ConsoleUtility::gotoxy(x, y+5);
		cout << "Moves: " << moves << endl;
		ConsoleUtility::setConsoleColors(4, 15);

		ConsoleUtility::gotoxy(x-10, y+10);
		cout << "*****************************************" << endl;
		ConsoleUtility::gotoxy(x-10, y+11);
		cout << "*                                       *" << endl;
		ConsoleUtility::gotoxy(x-10, y+12);
		cout << "*        Thank you for playing!          *" << endl;
		ConsoleUtility::gotoxy(x-10, y+13);
		cout << "*                                       *" << endl;
		ConsoleUtility::gotoxy(x-10, y+14);
		cout << "*****************************************" << endl;
		ConsoleUtility::setConsoleColors(0, 15);
	
		ConsoleUtility::gotoxy(0, 30);
	}

	static void printTableau(Node<Card>* temp, int x, int y) {
		Node<Card>* c = temp;
		if (!c) {
			ConsoleUtility::gotoxy(x, y);
			cout << "[Empty]";
		}
		while (c)
		{
			ConsoleUtility::gotoxy(x, y);
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
};

