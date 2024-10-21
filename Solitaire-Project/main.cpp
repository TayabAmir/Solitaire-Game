#include<iostream> 
#include<vector> 
#include<cstdlib> 
#include<ctime> 
#include<algorithm> 
#include<windows.h> 
#include "Stack.h" 
#include "Queue.h"
#include "Card.cpp" 
#include "LinkedList.cpp" 
#include "Node.cpp" 
using namespace std;

class SolitaireGame {
private:
    LinkedList<Card> tableaus[7];
    Stack<Card> foundations[4];
    Stack<Card> stockPile, wastePile;
   
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
        cout << "Stock Pile: ";
        if (stockPile.empty()) {
            cout << "Empty";
        }
        else {
            cout << stockPile.size() << " Cards";
        }
        // Position of WastePile below StockPile
        gotoxy(2, 3);
        cout << "Waste Pile: ";
        if (wastePile.empty()) {
            cout << "Empty";
        }
        else {
            wastePile.top().display();
        }

        // Foundations at top right
        gotoxy(70, 1); 
        cout << "Foundations:";
        for (int i = 0; i < 4; i++) {
            gotoxy(70 + i * 10, 3);
            if (foundations[i].empty()) {
                cout << "[Empty]";
            }
            else {
                foundations[i].top().display();
            }
        }

        int x = 5;
        int y = 6;
        for (int i = 0; i < 7; i++) {
            gotoxy(x + i * 15, y); 

            cout << "Tableau " << i + 1;
            printTableau(tableaus[i].getHead(), x + i * 15, y + 2);
        }
    }

    void printTableau(Node<Card>* temp, int x, int y) {

        Node<Card>* c = reverseList(temp);

        while(c)
        {
            gotoxy(x, y);
            if (c->val.isFaceUp)
                c->val.display();
            else
                cout << "Card Down" << endl;
            c = c->next;
            y++;
        }
    }

    Node<Card>* reverseList(Node<Card>* temp) {
        LinkedList<Card> reversedList;

        Node<Card>* current = temp;

        while (current != NULL) {
            reversedList.insertAtHead(current->val);
            current = current->next;
        }

        return reversedList.getHead();
    }


    bool isValidMove(Card& c, Card& topCard) {
        // Conditions for valid move i.e different colors
        if ((c.rank == topCard.rank - 1) && ((c.suit == "Hearts" || c.suit == "Diamonds") && (topCard.suit == "Clubs" || topCard.suit == "Spades")) || ((c.suit == "Clubs" || c.suit == "Spades") && (topCard.suit == "Hearts" || topCard.suit == "Diamonds")))
            return true;
        return false;
    }

    void drawCard() {
        // If stock pile is empty, put all cards from wastepile to stockpile
        if (stockPile.empty()) {
            while (!wastePile.empty()) {
                stockPile.push(wastePile.top());
                wastePile.pop();
            }
        }
        // If stockpile not empty take card from it
        else {
            wastePile.push(stockPile.top());
            stockPile.pop();
            wastePile.top().isFaceUp = true;
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
    void moveCardBetweenTableaus(LinkedList<Card>& from, LinkedList<Card>& to,int noOfCards) {
        Node<Card>* head = from.getHead();
        for (int i = 0; i < noOfCards; ++i) {
            if (!head->val.isFaceUp) {
                cout << "Cannot move not visible Cards";
                return;
            }
        }
        for (int i = 0; i < noOfCards; i++) {
            Card temp = from.getHead()->val;
            from.deleteFromStart();
            to.insertAtHead(temp);
        }
        from.getHead()->val.isFaceUp = true;
    }
    void moveCardFromWtoF(Stack<Card>& w, Stack<Card>& f) {
        Card c = w.top();
        w.pop();
        f.push(c);
    }

    void moveCarFromTtoF(LinkedList<Card>& t, Stack<Card>& f) {
        Card c = t.getHead()->val;
        f.push(c);
        t.deleteFromStart();
    }
    void moveCardFromWtoT(Stack<Card>& w, LinkedList<Card>& t) {
        Card c = w.top();
        w.pop();
        t.insertAtHead(c);
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
                tableaus[tableauIdx].insertAtHead(cards[cardIdx]);
                if (card == tableauIdx + 1)
                    tableaus[tableauIdx].getHead()->val.isFaceUp = true;
                cardIdx++;
            }
        }

        // Filling StockPile with Cards
        while (cardIdx < cards.size()) {
            stockPile.push(cards[cardIdx++]);
        }
        stockPile.push(Card(1, 1));
    }

    void play() {
        string cmd;
        while (true) {
            system("Color 70");
            displayGame();

            gotoxy(2, 25);
            cout << "Enter command (move, draw, quit): ";
            cin >> cmd;

                if (cmd == "move") {
                    int from, fromIdx, to, toIdx = -1;
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

                    if (from == 1 && fromIdx >= 1 && fromIdx <= 7) {
                        // From Tableau to Tableau
                        if (to == 1 && toIdx >= 1 && toIdx <= 7) {
                            int noOfCards;
                            cout << "Enter Number of Cards you want to move: ";
                            cin >> noOfCards;
                            if (tableaus[fromIdx - 1].size() < noOfCards) {
                                cout << "There are not enough cards in Tableau " << fromIdx << "." << endl;
                                continue;
                            }
                            Node<Card>* uppestCard = tableaus[fromIdx - 1].getHead();;
                            if (noOfCards > 1) {
                                while (uppestCard->next && uppestCard->next->val.isFaceUp)
                                    uppestCard = uppestCard->next;
                            }

                            if (!isValidMove(uppestCard->val, tableaus[toIdx - 1].getHead()->val)) {
                                cout << "Invalid Move." << endl;
                                continue;
                            }

                            moveCardBetweenTableaus(tableaus[fromIdx - 1], tableaus[toIdx - 1], noOfCards);
                        }
                        // From Tableau to Foundation
                        else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
                            if (tableaus[fromIdx - 1].size() >= 1 && isValidMove(tableaus[fromIdx - 1].getHead()->val, foundations[toIdx - 1].top())) {
                                moveCarFromTtoF(tableaus[fromIdx - 1], foundations[toIdx - 1]);
                            } 
                        }
                        else {
                            cout << "Invalid destination!" << endl;
                        }
                    }
                    else if (from == 2) {
                        // From WastePile to Tableau
                        if (to == 1 && toIdx >= 1 && toIdx <= 7) {
                            if(!wastePile.empty() && isValidMove(wastePile.top(), tableaus[toIdx - 1].getHead()->val))
                                moveCardFromWtoT(wastePile, tableaus[toIdx - 1]);
                        }
                        // From WastePile to Foundation
                        else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
                            if (!wastePile.empty() && ((wastePile.top().rank == 1 && foundations[toIdx - 1].empty()) || isValidMove(wastePile.top(), foundations[toIdx - 1].top())))
                                moveCardFromWtoF(wastePile, foundations[toIdx - 1]);
                        }
                        else {
                            cout << "Invalid destination!" << endl;
                        }
                    }
                    else {
                        cout << "Invalid source!" << endl;
                    }
                }
            else if (cmd == "draw") {
                drawCard();
            }
            else if (cmd == "quit") {
                break;
            }
            else {
                cout << "Invalid command!" << endl;
            }
            if (win()) {
                cout << "Congratulations! You've won the game!" << endl;
                break;
            }
        }
    }
};


int main() {

    SolitaireGame game;
    game.play();
    return 0;
}