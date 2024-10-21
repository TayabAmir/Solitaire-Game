void displayGame() {
        system("cls");
        system("Color 70");


        // StockPile and WastePile at top left
        gotoxy(2, 1);  // Position of StockPile
        cout << "Stock Pile: ";
        if (stockPile.empty()) {
            cout << "Empty";
        }
        else {
            cout << stockPile.size() << " Cards";
        }

        gotoxy(2, 3);  // Position of WastePile below StockPile
        cout << "Waste Pile: ";
        if (wastePile.empty()) {
            cout << "Empty";
        }
        else {
            wastePile.top().display();
        }

        // Foundations at top right
        gotoxy(70, 1);  // Starting position of Foundations
        cout << "Foundations:";
        for (int i = 0; i < 4; i++) {
            gotoxy(70 + i * 10, 3);  // Place each foundation 10 spaces apart
            if (foundations[i].empty()) {
                cout << "[Empty]";
            }
            else {
                foundations[i].top().display();
            }
        }

        // Tableaus centered
        int startingX = 5;  // Starting X position for Tableaus
        int startingY = 6;  // Starting Y position for Tableaus
        for (int i = 0; i < 7; i++) {
            gotoxy(startingX + i * 15, startingY);  // Place each tableau 12 spaces apart

            cout << "Tableau " << i + 1;
            printTableau(tableaus[i].getHead(), startingX + i * 15, startingY + 2);  // Call helper function to print the cards below the label
        }
    }

    void printTableau(Node<Card>* temp, int x, int y) {
        if (!temp) {
            return;
        }
        printTableau(temp->next, x, y+1);
        gotoxy(x, y);
        if (temp->val.isFaceUp)
            temp->val.display();
        else
            cout << "Card Down" << endl;
    }
