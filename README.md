#  **ADVANCED TIC TAC TOE – DETAILED DOCUMENTATION WITH OUTPUT**

---

##  **1. INTRODUCTION**

This project is a **console-based Tic Tac Toe game** developed in **C++ using Object-Oriented Programming (OOP)**.
It supports multiple gameplay modes and an intelligent AI using the **Minimax algorithm with Alpha-Beta pruning**.

---

##  **2. OBJECTIVE**

* To design a modular Tic Tac Toe system
* To implement AI-based decision making
* To demonstrate OOP concepts like abstraction, inheritance, polymorphism

---

##  **3. SYSTEM ARCHITECTURE**

###  Classes Used:

* `GameBoard` → Handles grid logic
* `GameRenderer` → Handles display
* `Player` → Base class
* `HumanPlayer` → User input
* `AIPlayer` → AI logic
* `GameEngine` → Controls flow

---

##  **4. WORKING FLOW**

1. Menu displayed
2. User selects mode
3. Players initialized
4. Game loop starts
5. Moves executed turn by turn
6. Game ends (Win/Draw)
7. Replay or exit

---

#  **5. OUTPUT SCREENSHOTS WITH EXPLANATION**

---

##  **OUTPUT 1: MAIN MENU**

```
 TIC TAC TOE MAIN MENU
=============================
1. Play as X (go first)
2. Play as O (go second)
3. Watch AI vs AI
4. Quit
Enter choice (1-4):
```

###  Explanation:

* This is the **entry point**
* User selects game mode
* Controls entire flow

---

##  **OUTPUT 2: SELECT OPPONENT**

```
Choose opponent:
1. Human
2. AI (Easy)
3. AI (Medium)
4. AI (Impossible)
```

###  Explanation:

* User selects opponent type
* Difficulty affects AI intelligence

---

##  **OUTPUT 3: GAME BOARD INITIAL STATE**

```
 ADVANCED TIC TAC TOE

  ┌───┬───┬───┐
  │ 0 │ 1 │ 2 │
  ├───┼───┼───┤
0 │   │   │   │
  ├───┼───┼───┤
1 │   │   │   │
  ├───┼───┼───┤
2 │   │   │   │
  └───┴───┴───┘

 Enter moves as 'row,col' (0-2)
 Example: '0,1' for top middle
```

###  Explanation:

* Displays empty board
* Coordinates help user input correctly

---

##  **OUTPUT 4: HUMAN MOVE INPUT**

```
Human (X) - Enter row,col (0-2): 0 0
```

###  Explanation:

* User enters position
* System validates input
* If invalid → asks again

---

##  **OUTPUT 5: BOARD AFTER MOVE**

```
  ┌───┬───┬───┐
0 │ X │   │   │
  ├───┼───┼───┤
1 │   │   │   │
  ├───┼───┼───┤
2 │   │   │   │
  └───┴───┴───┘
```

###  Explanation:

* Move is updated
* Board refreshed visually

---

##  **OUTPUT 6: AI THINKING**

```
 AI is thinking...
```

###  Explanation:

* Simulates AI processing
* Internally runs minimax algorithm

---

##  **OUTPUT 7: GAME PROGRESSION**

```
0 │ X │ O │ X │
1 │ O │ X │   │
2 │   │   │ O │
```

###  Explanation:

* Shows mid-game state
* Both players making moves

---

##  **OUTPUT 8: WIN CONDITION**

```
 PLAYER X WINS!
 Congratulations X!
```

###  Explanation:

* Triggered when:

  * Row match OR
  * Column match OR
  * Diagonal match

---

##  **OUTPUT 9: DRAW CONDITION**

```
 IT'S A DRAW!
 Well played!
```

###  Explanation:

* No empty cells
* No winner

---

##  **OUTPUT 10: POST GAME MENU**

```
 GAME OVER - What next?
1   Play Again (same settings)
2   Change Settings
3   Main Menu
4   Quit
Enter choice (1-4):
```

###  Explanation:

* Allows replay or exit
* Improves user experience

---

#  **6. AI LOGIC EXPLANATION**

###  Minimax:

* Explores all possible moves
* Assigns scores:

  * Win → +10
  * Loss → -10
  * Draw → 0

###  Alpha-Beta Pruning:

* Skips unnecessary branches
* Improves performance

---

#  **7. FEATURES**

* OOP-based design
* Smart AI (3 levels)
* Input validation
* Multiple modes
* Clean UI (console)

---

#  **8. ADVANTAGES**

* Easy to maintain
* Scalable
* Demonstrates AI concepts
* Efficient decision making

---

#  **9. LIMITATIONS**

* No GUI
* Fixed 3x3 board
* No online play

---

#  **10. FUTURE ENHANCEMENTS**

* GUI version
* Multiplayer online
* Score tracking
* Sound effects

---

#  **11. CONCLUSION**

This project successfully implements a **feature-rich Tic Tac Toe game** using:

* OOP principles
* AI algorithms
* Clean modular design
