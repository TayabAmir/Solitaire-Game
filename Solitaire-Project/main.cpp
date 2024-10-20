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

using namespace std;

class SolitaireGame {
private:
    Stack<Card> tableau[7];
    Stack<Card> foundations[4];
    Stack<Card> stockPile, wastePile;
   
    void displayGame() {
        cout << "=== Solitaire Game ===" << endl << endl;

        // Showing Foundations
        cout << "Foundations:" << endl;
        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ": ";
            if (foundations[i].empty()) {
                cout << "Empty";
            }
            else {
                foundations[i].top().display();
            }
            cout << endl;
        }
        cout << endl;


        // Showing Tableaus
        cout << "Tableau:" << endl;
        for (int i = 0; i < 7; i++) {
            cout << "    " << i + 1 << ": \n";
            printTableau(tableau[i]);
            cout << endl;
        }
        cout << endl;

        // Showing Stock Pile
        if (stockPile.empty()) {
            cout << "Stock Pile: Empty" << endl;
        }
        else {
            cout << "Stock Pile: " << stockPile.size() << " Cards" << endl;
        }

        // Showing Waste Pile
        if (wastePile.empty()) {
            cout << "Waste Pile: Empty" << endl;
        }
        else {
            cout << "Waste Pile: ";
            wastePile.top().display();
        }
        cout << endl;
    }

    void printTableau(Stack<Card>& t) {
        // lS is list of cards to show and lH is list of cards to hide.... Will be helpful while moving multiple cards and shwoing them
        Node<Card>* lH = t.getHideList().getHead(); 
        Node<Card>* lS = t.getShowList().getHead();

        while (lH) {
            cout << "Card Down" << endl;
            lH = lH->next;
        }
        while (lS) {
            lS->val.display();
            lS = lS->next;
        }
    }

    bool isValidMove(Card& c, Stack<Card>& des) {
        // If it is a king then it can be place in empty tableau or If it is a A card then it can be place in foundation
        
        if (des.empty()) { 
            return c.rank == 13 || c.rank == 1;
        }
        Card topCard = des.top();
        // Conditions for valid move i.e different colors
        if ((c.rank == topCard.rank - 1) && ((c.suit == "Hearts" || c.suit == "Diamonds") && (topCard.suit == "Clubs" || topCard.suit == "Spades")) || ((c.suit == "Clubs" || c.suit == "Spades") && (topCard.suit == "Hearts" || topCard.suit == "Diamonds")))
            return true;
        return false;
    }

    void moveCard(Stack<Card>& st, Stack<Card>& end) {
        // First checking if it is a valid move, if it is then moving card
        if (!st.empty() && isValidMove(st.top(), end)) {
            end.push(st.top());
            st.pop();
            if (!st.empty()) {
                st.top().isFaceUp = true;
            }
        }
        else {
            cout << "Invalid move!" << endl;
        }
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
        int tableauIdx = 0, cardIdx = 0;
        for (auto& st : tableau) {
            for (int card = 1; card <= tableauIdx + 1; card++) {
                st.push(cards[cardIdx++]);
            }
            Card topCard = st.top();

            topCard.isFaceUp = true;

            st.pop();
            st.push(topCard);
            tableauIdx++;
        }

        // Filling StockPile with Cards
        while (cardIdx < cards.size()) {
            stockPile.push(cards[cardIdx++]);
        }
    }

    void play() {
        string cmd;
        while (true) {
            displayGame();
            cout << "Enter command (move, draw, quit): ";
            cin >> cmd;

            if (cmd == "move") {
                int from, fromIdx, to, toIdx;
                cout << "Enter source (1-Tableau, 2-Waste Pile) and index: ";
                cin >> from >> fromIdx;
                cout << "Enter destination (1-Tableau, 2-Foundation) and index: ";
                cin >> to >> toIdx;

                if (cmd == "move") {
                    int from, fromIdx, to, toIdx;
                    cout << "Enter source (1 for Tableau, 2 for Waste Pile) and index: ";
                    cin >> from >> fromIdx;
                    cout << "Enter destination (1 for Tableau, 2 for Foundation) and index: ";
                    cin >> to >> toIdx;

                    if (from == 1 && fromIdx >= 1 && fromIdx <= 7) {
                        // From Tableau to Tableau
                        if (to == 1 && toIdx >= 1 && toIdx <= 7) {
                            moveCard(tableau[fromIdx - 1], tableau[toIdx - 1]);
                        }
                        // From Tableau to Foundation
                        else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
                            moveCard(tableau[fromIdx - 1], foundations[toIdx - 1]);
                        }
                        else {
                            cout << "Invalid destination!" << endl;
                        }
                    }
                    else if (from == 2) {
                        // From WastePile to Tableau
                        if (to == 1 && toIdx >= 1 && toIdx <= 7) {
                            moveCard(wastePile, tableau[toIdx - 1]);
                        }
                        // From WastePile to Foundation
                        else if (to == 2 && toIdx >= 1 && toIdx <= 4) {
                            moveCard(wastePile, foundations[toIdx - 1]);
                        }
                        else {
                            cout << "Invalid destination!" << endl;
                        }
                    }
                    else {
                        cout << "Invalid source!" << endl;
                    }
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