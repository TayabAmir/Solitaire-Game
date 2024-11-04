#include<iostream> 
#include<vector> 
#include<cstdlib> 
#include<ctime> 
#include<algorithm> 
#include<conio.h> 
#include<windows.h> 
#include<unordered_map> 
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
    Queue<Card> stockPile, wastePile;
   
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void setConsoleColors(int textColor, int backgroundColor) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (backgroundColor << 4) | textColor);
    }


    void displayGame() {
        system("cls");
        // StockPile and WastePile at top left
        gotoxy(2, 1);
        cout << "Stock Pile: ";
        if (stockPile.empty()) {
            drawCard();
            cout << "24 cards";
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
            Queue<Card> temp = wastePile;
            while (temp.size() > 1) {
                temp.dequeue();
            }
            temp.peek().display();
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
            printTableau(tableaus[i].head, x + i * 15, y + 2);
        }
    }

    void printTableau(Node<Card>* temp, int x, int y) {
        Node<Card>* c = temp;
        while(c)
        {
            gotoxy(x, y);
            if (c->val.isFaceUp)
            {
                if(c->val.suit == "Hearts" || c->val.suit == "Diamonds")
                    setConsoleColors(12, 15);
                c->val.display();
                setConsoleColors(0, 15);
            }
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

        return reversedList.head;
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
                stockPile.enqueue(wastePile.peek());
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
    void moveCardBetweenTableaus(LinkedList<Card>& from, LinkedList<Card>& to,int noOfCards) {
        Node<Card>* head = from.head;
        int s = from.size();
        for (int i = 0; i < s - noOfCards - 1; i++) {
            head = head->next;
        }
        to.tail->next = head->next;
        head->next = NULL;
        to.tail = from.tail;
        from.tail = head;
        from.tail->val.isFaceUp = true;
    }

    void moveCardFromWtoF(Queue<Card>& w, Stack<Card>& f) {
        Queue<Card> temp;
        while (w.size() > 1){
            temp.enqueue(w.peek());
            w.dequeue();
        }
        Card c = w.peek();
        w.dequeue();
        if ((c.rank == 1 && f.empty()) || isValidMove(c, f.top())) {
            c.isFaceUp = true;
            f.push(c);
        }
        else {
            cout << "Invalid Move";
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
        if(t.tail)
        t.tail->val.isFaceUp = true;
    }
    void moveCardFromWtoT(Queue<Card>& w, LinkedList<Card>& t) {
        Queue<Card> temp;
        while (w.size() > 1){
            temp.enqueue(w.peek());
            w.dequeue();
        }
        Card c = w.peek();
        w.dequeue();
        if (isValidMove(c, t.tail->val)) {
            c.isFaceUp = true;
            t.insertAtEnd(c);
        }
        else {
            cout << "Invalid Move";
        }
        while (!temp.empty()) {
            w.enqueue(temp.peek());
            temp.dequeue();
        }
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

    void play() {
        string cmd;
        while (true) {
            setConsoleColors(0, 15);
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
                            if (noOfCards > noofFaceupCards(tableaus[fromIdx - 1])) {
                                cout << "Not enough face up cards." << endl;
                                continue;
                            }
                            Node<Card>* head = tableaus[fromIdx - 1].head;
                            for (int i = 0; i < tableaus[fromIdx - 1].size() - noOfCards; i++) {
                                head = head->next;
                            }

                            if (!isValidMove(head->val, tableaus[toIdx - 1].tail->val)) {
                                cout << "Invalid Move." << endl;
                                continue;
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
                                }
                                else if (isValidMove(foundations[toIdx - 1].top(), c)) {
                                    moveCarFromTtoF(tableaus[fromIdx - 1], foundations[toIdx - 1]);
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

            char ch = _getch();

        }
    }
};


int main() {

    SolitaireGame game;
    game.play();
    return 0;
}