- Solitaire Project

This project is a C++ implementation of the Klondike Solitaire game. The game includes various components such as cards, moves, and data structures like linked lists, stacks, and queues to manage the game state.

- Dependencies

C++ Compiler
If you are using VS Community, ensure you have the C++ development workload installed.

- Game Components

Tableau: Tableau is implemented with seven linked lists.
Foundation: Foundation is implemented with four stacks.
Stock: Stock pile is implemented with a Queue.
Waste: Waste pile is implemented with a Queue.
Card: Represents a playing card.
Move: Represents a move in the game.
Console Utility: Contains functions used in multiple classes.
UI: Handles the user interface.

- Main Program

Contains the entry point of the program. The file from where the project starts.


                                             <----------- How to Play --------->

- The game will initialize and display the initial layout of the cards.

Game Commands:

   --- In source 1 represents a tableau, and 2 represents a waste pile and for destination 1 represents tableau and 2 represents foundation. When you select tableau and foundation, you also have to specify the number. And for moving cards between the tableaus, you also have to tell the number of cards.
    
   *Enter the commands line by line as the game asks you*

   - Moving cards between tableaus: For example, to move 2 cards from tableau 2 to tableau 5, you might use a command like `move 1 2 1 5 2`.
   - Move from Waste to Foundation: For example, to move card from waste to foundation 3, use a command like `move 2 2 3`.
   - Move from Tableau to Foundation: To move a card from tableau 2 to foundation 4, use a command like `move 1 2 2 4`.
   - Move from Waste to Tableau: To move a card from the waste pile to a tableau 4, use a command like `move 2 1 4`.
   - Draw a Card: To draw a card from the stock pile to the waste pile, use the command `draw`.
   - Undo a Move: To undo the last move, use the command `undo`.
   - Redo a Move: To redo the last undone move, use the command `redo`.
   - Quit game: To quit the game, use the command `quit`.

Winning the Game:
   - The game is won when all cards are moved to the foundation piles in the correct order (Ace to King for each suit).

Game Loop:
   - The game will continue to prompt you for commands until you win the game or decide to quit.
