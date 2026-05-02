#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>
#include <thread>
#include <memory>
#include <random>
using namespace std;

enum class PlayerType { HUMAN, AI };
enum class GameState { ONGOING, X_WON, O_WON, DRAW };
enum class Difficulty { EASY, MEDIUM, HARD };

const int BOARD_SIZE = 3;
const char EMPTY = ' ';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

class GameBoard;
class GameRenderer;

class GameBoard {
private:
    char grid[BOARD_SIZE][BOARD_SIZE];
    
    bool checkRowWin(int row, char player) const;
    bool checkColWin(int col, char player) const;
    bool checkDiagWin(char player) const;
    
public:
    GameBoard() { reset(); }
    
    void reset();
    bool isValidMove(int row, int col) const;
    bool makeMove(int row, int col, char player);
    GameState getGameState() const;
    vector<pair<int, int>> getAvailableMoves() const;
    char getCell(int row, int col) const { return grid[row][col]; }
    bool isFull() const;
};

class GameRenderer {
public:
    static void renderBoard(const GameBoard& board);
    static void renderGameState(GameState state, char winner);
    static void renderAIThinking(Difficulty diff);
    static void clearScreen();
    static void renderMenu();
};

class Player {
protected:
    char symbol;
    PlayerType type;
    
public:
    Player(char sym, PlayerType t) : symbol(sym), type(t) {}
    virtual ~Player() = default;
    
    char getSymbol() const { return symbol; }
    PlayerType getType() const { return type; }
    virtual pair<int, int> getMove(const GameBoard& board) = 0;
    virtual string getName() const = 0;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(char sym) : Player(sym, PlayerType::HUMAN) {}
    
    pair<int, int> getMove(const GameBoard& board) override {
        int row, col;
        while (true) {
            cout << "\n" << getName() << " (" << symbol << ") - Enter row,col (0-2): ";
            cin >> row >> col;
            
            if (board.isValidMove(row, col)) {
                return {row, col};
            }
            cout << " Invalid move! Try again." << endl;
        }
    }
    
    string getName() const override { return "Human"; }
};

class AIPlayer : public Player {
private:
    Difficulty difficulty;
    int searchDepth;
    mt19937 rng;
    
    int minimax(const GameBoard& board, int depth, bool isMaximizing, 
                int alpha, int beta, char maximizingPlayer) const;
    
    pair<int, int> findBestMove(const GameBoard& board) const;
    
public:
    AIPlayer(char sym, Difficulty diff = Difficulty::HARD);
    pair<int, int> getMove(const GameBoard& board) override;
    string getName() const override;
};

class GameEngine {
private:
    GameBoard board;
    unique_ptr<Player> playerX;
    unique_ptr<Player> playerO;
    GameRenderer renderer;
    Player* currentPlayer;
    char humanSym;
    PlayerType opponentType;
    Difficulty aiDiff;
    
    GameState playSingleGame();
    
public:
    GameEngine(char humanSym, PlayerType opponentType, Difficulty aiDiff = Difficulty::HARD);
    void run();
    void restartGame();
};

void GameBoard::reset() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            grid[i][j] = EMPTY;
}

bool GameBoard::isValidMove(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && grid[row][col] == EMPTY;
}

bool GameBoard::makeMove(int row, int col, char player) {
    if (isValidMove(row, col)) {
        grid[row][col] = player;
        return true;
    }
    return false;
}

bool GameBoard::checkRowWin(int row, char player) const {
    return grid[row][0] == player && grid[row][1] == player && grid[row][2] == player;
}

bool GameBoard::checkColWin(int col, char player) const {
    return grid[0][col] == player && grid[1][col] == player && grid[2][col] == player;
}

bool GameBoard::checkDiagWin(char player) const {
    return (grid[0][0] == player && grid[1][1] == player && grid[2][2] == player) ||
           (grid[0][2] == player && grid[1][1] == player && grid[2][0] == player);
}

GameState GameBoard::getGameState() const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (checkRowWin(i, PLAYER_X)) return GameState::X_WON;
        if (checkRowWin(i, PLAYER_O)) return GameState::O_WON;
    }
    
    for (int j = 0; j < BOARD_SIZE; j++) {
        if (checkColWin(j, PLAYER_X)) return GameState::X_WON;
        if (checkColWin(j, PLAYER_O)) return GameState::O_WON;
    }
    
    if (checkDiagWin(PLAYER_X)) return GameState::X_WON;
    if (checkDiagWin(PLAYER_O)) return GameState::O_WON;
    
    return isFull() ? GameState::DRAW : GameState::ONGOING;
}

vector<pair<int, int>> GameBoard::getAvailableMoves() const {
    vector<pair<int, int>> moves;
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (grid[i][j] == EMPTY)
                moves.emplace_back(i, j);
    return moves;
}

bool GameBoard::isFull() const {
    return getAvailableMoves().empty();
}

void GameRenderer::clearScreen() {
    cout << "\033[2J\033[H";
}

void GameRenderer::renderBoard(const GameBoard& board) {
    clearScreen();
    cout << " ADVANCED TIC TAC TOE " << endl << endl;
    
    cout << "  ┌───┬───┬───┐" << endl;
    cout << "  │ 0 │ 1 │ 2 │" << endl;
    cout << "  ├───┼───┼───┤" << endl;
    cout << "0 │ " << board.getCell(0,0) << " │ " << board.getCell(0,1) << " │ " << board.getCell(0,2) << " │" << endl;
    cout << "  ├───┼───┼───┤" << endl;
    cout << "1 │ " << board.getCell(1,0) << " │ " << board.getCell(1,1) << " │ " << board.getCell(1,2) << " │" << endl;
    cout << "  ├───┼───┼───┤" << endl;
    cout << "2 │ " << board.getCell(2,0) << " │ " << board.getCell(2,1) << " │ " << board.getCell(2,2) << " │" << endl;
    cout << "  └───┴───┴───┘" << endl << endl;
    
    cout << " Enter moves as 'row,col' (0-2)" << endl;
    cout << " Example: '0,1' for top middle" << endl << endl;
}

void GameRenderer::renderGameState(GameState state, char winner) {
    cout << endl;
    switch (state) {
        case GameState::X_WON: 
            cout << " PLAYER X WINS! " << endl 
                 << " Congratulations " << winner << "!" << endl; 
            break;
        case GameState::O_WON: 
            cout << " PLAYER O WINS! " << endl 
                 << " Congratulations " << winner << "!" << endl; 
            break;
        case GameState::DRAW:  
            cout << " IT'S A DRAW! " << endl 
                 << "  Well played!" << endl; 
            break;
        default: break;
    }
    cout << endl;
}

void GameRenderer::renderAIThinking(Difficulty diff) {
    cout << "\n " << ((diff == Difficulty::HARD) ? "Impossible AI" : "AI") 
         << " is thinking";
    cout.flush();
    for (int i = 0; i < 3; i++) {
        cout << "." << flush;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    cout << endl;
}

void GameRenderer::renderMenu() {
    cout << "\n TIC TAC TOE MAIN MENU " << endl;
    cout << "=============================" << endl;
    cout << "1. Play as X (go first)" << endl;
    cout << "2. Play as O (go second)" << endl;
    cout << "3. Watch AI vs AI" << endl;
    cout << "4. Quit" << endl;
    cout << "Enter choice (1-4): ";
}

AIPlayer::AIPlayer(char sym, Difficulty diff) 
    : Player(sym, PlayerType::AI), difficulty(diff), 
      rng(chrono::steady_clock::now().time_since_epoch().count()) {
    searchDepth = (diff == Difficulty::EASY) ? 4 : 
                 (diff == Difficulty::MEDIUM) ? 6 : 9;
}

int AIPlayer::minimax(const GameBoard& board, int depth, bool isMaximizing, 
                     int alpha, int beta, char maximizingPlayer) const {
    GameState state = board.getGameState();
    
    if (state == GameState::X_WON && maximizingPlayer == PLAYER_X) return 10 - depth;
    if (state == GameState::O_WON && maximizingPlayer == PLAYER_O) return 10 - depth;
    if (state == GameState::X_WON) return depth - 10;
    if (state == GameState::O_WON) return depth - 10;
    if (board.isFull()) return 0;
    
    if (isMaximizing) {
        int maxEval = numeric_limits<int>::min();
        for (auto [row, col] : board.getAvailableMoves()) {
            GameBoard tempBoard = board;
            tempBoard.makeMove(row, col, maximizingPlayer);
            int eval = minimax(tempBoard, depth + 1, false, alpha, beta, maximizingPlayer);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = numeric_limits<int>::max();
        char opponent = (maximizingPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
        for (auto [row, col] : board.getAvailableMoves()) {
            GameBoard tempBoard = board;
            tempBoard.makeMove(row, col, opponent);
            int eval = minimax(tempBoard, depth + 1, true, alpha, beta, maximizingPlayer);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

pair<int, int> AIPlayer::findBestMove(const GameBoard& board) const {
    int bestScore = numeric_limits<int>::min();
    pair<int, int> bestMove = {-1, -1};
    
    for (auto [row, col] : board.getAvailableMoves()) {
        GameBoard tempBoard = board;
        tempBoard.makeMove(row, col, symbol);
        int score = minimax(tempBoard, 0, false, 
                           numeric_limits<int>::min(), 
                           numeric_limits<int>::max(), symbol);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = {row, col};
        }
    }
    return bestMove;
}

pair<int, int> AIPlayer::getMove(const GameBoard& board) {
    GameRenderer::renderAIThinking(difficulty);
    this_thread::sleep_for(chrono::milliseconds(500));
    
    if (difficulty == Difficulty::EASY) {
        auto moves = board.getAvailableMoves();
        shuffle(moves.begin(), moves.end(), rng);
        return moves[0];
    }
    return findBestMove(board);
}

string AIPlayer::getName() const {
    string diffStr;
    if (difficulty == Difficulty::EASY) {
        diffStr = "Easy";
    } else if (difficulty == Difficulty::MEDIUM) {
        diffStr = "Medium";
    } else {
        diffStr = "Impossible";
    }
    return "AI (" + diffStr + ")";
}

GameEngine::GameEngine(char humanSym, PlayerType opponentType, Difficulty aiDiff) 
    : currentPlayer(nullptr), humanSym(humanSym), opponentType(opponentType), aiDiff(aiDiff) {
    
    if (humanSym == PLAYER_X) {
        playerX = make_unique<HumanPlayer>(PLAYER_X);
        if (opponentType == PlayerType::HUMAN) {
            playerO = make_unique<HumanPlayer>(PLAYER_O);
        } else {
            playerO = make_unique<AIPlayer>(PLAYER_O, aiDiff);
        }
        currentPlayer = playerX.get();
    } else {
        if (opponentType == PlayerType::HUMAN) {
            playerX = make_unique<HumanPlayer>(PLAYER_X);
        } else {
            playerX = make_unique<AIPlayer>(PLAYER_X, aiDiff);
        }
        playerO = make_unique<HumanPlayer>(PLAYER_O);
        currentPlayer = playerX.get();
    }
}

GameState GameEngine::playSingleGame() {
    while (board.getGameState() == GameState::ONGOING) {
        renderer.renderBoard(board);
        cout << currentPlayer->getName() << "'s turn (" << currentPlayer->getSymbol() << ")" << endl;
        auto [row, col] = currentPlayer->getMove(board);
        board.makeMove(row, col, currentPlayer->getSymbol());
        
        currentPlayer = (currentPlayer == playerX.get()) ? playerO.get() : playerX.get();
    }
    
    renderer.renderBoard(board);
    return board.getGameState();
}

void GameEngine::restartGame() {
    board.reset();
    currentPlayer = (humanSym == PLAYER_X) ? playerX.get() : playerX.get();
}

void GameEngine::run() {
    char choice;
    do {
        restartGame(); 
        GameState result = playSingleGame();
        renderer.renderGameState(result, (result == GameState::X_WON) ? PLAYER_X : PLAYER_O);
        
        cout << "\n GAME OVER - What next?" << endl;
        cout << "1   Play Again (same settings)" << endl;
        cout << "2   Change Settings" << endl;
        cout << "3   Main Menu" << endl;
        cout << "4   Quit" << endl;
        cout << "Enter choice (1-4): ";
        cin >> choice;
        
        if (choice == '2' || choice == '3') {
            return;  
        }
    } while (choice == '1');
    
    cout << "\nReturning to main menu... " << endl;
}

int main() {
    while (true) {
        GameRenderer::renderMenu();
        int mainChoice;
        cin >> mainChoice;
        
        if (mainChoice == 4) {
            cout << "\n Thanks for playing! " << endl;
            break;
        }
        
        char humanSymbol = PLAYER_X;
        PlayerType opponentType = PlayerType::HUMAN;
        Difficulty aiDiff = Difficulty::HARD;
        bool isAIvsAI = false;
        
        if (mainChoice == 1) {
            humanSymbol = PLAYER_X;
            cout << "\nChoose opponent:\n";
            cout << "1. Human\n";
            cout << "2. AI (Easy)\n";
            cout << "3. AI (Medium)\n";
            cout << "4. AI (Impossible)\n";
            int choice;
            cin >> choice;
            switch (choice) {
                case 1: opponentType = PlayerType::HUMAN; break;
                case 2: opponentType = PlayerType::AI; aiDiff = Difficulty::EASY; break;
                case 3: opponentType = PlayerType::AI; aiDiff = Difficulty::MEDIUM; break;
                case 4: opponentType = PlayerType::AI; aiDiff = Difficulty::HARD; break;
                default: opponentType = PlayerType::AI; aiDiff = Difficulty::HARD; break;
            }
        } 
        else if (mainChoice == 2) {
            humanSymbol = PLAYER_O;
            cout << "\nChoose opponent AI difficulty:\n";
            cout << "1. Easy\n";
            cout << "2. Medium\n";
            cout << "3. Impossible\n";
            int choice;
            cin >> choice;
            aiDiff = (choice == 1) ? Difficulty::EASY : 
                    (choice == 2) ? Difficulty::MEDIUM : Difficulty::HARD;
            opponentType = PlayerType::AI;
        }
        else if (mainChoice == 3) {
            isAIvsAI = true;
            cout << "\nChoose AI difficulties:\n";
            cout << "X AI: 1.Easy 2.Medium 3.Impossible -> ";
            int xDiff; cin >> xDiff;
            cout << "O AI: 1.Easy 2.Medium 3.Impossible -> ";
            int oDiff; cin >> oDiff;
            
            GameEngine engine(PLAYER_X, PlayerType::AI, 
                            (xDiff==1)?Difficulty::EASY:
                            (xDiff==2)?Difficulty::MEDIUM:Difficulty::HARD);
            auto* enginePtr = (void*)&engine;
            aiDiff = (oDiff==1)?Difficulty::EASY:
                    (oDiff==2)?Difficulty::MEDIUM:Difficulty::HARD;
            engine.run();
            continue;
        }
        else {
            cout << "Invalid choice! Starting X vs Human...\n";
        }
        
        if (!isAIvsAI) {
            cout << "\n You are " << humanSymbol << " | Opponent: ";
            if (opponentType == PlayerType::HUMAN) {
                cout << "Human";
            } else {
                string diffStr = (aiDiff == Difficulty::EASY) ? "Easy" : 
                               (aiDiff == Difficulty::MEDIUM) ? "Medium" : "Impossible";
                cout << "AI (" << diffStr << ")";
            }
            cout << endl;
            cout << ((humanSymbol == 'X') ? "You go first!" : "Opponent goes first!") << endl << endl;
            
            GameEngine engine(humanSymbol, opponentType, aiDiff);
            engine.run();
        }
    }
    
    return 0;
}