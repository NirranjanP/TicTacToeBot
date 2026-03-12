#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
enum Flag
{
    EMPTY,
    EXACT,
    LOWERBOUND,
    UPPERBOUND
};
struct position
{
    int score;
    Flag flag;
};
position TT[1 << 18];
uint16_t winningpatterns[] = {0b111000000, 0b000111000, 0b000000111,
                              0b100100100, 0b010010010, 0b001001001,
                              0b100010001, 0b001010100};
bool noMoveLeft(uint16_t X, uint16_t O)
{
    return ((X | O) == 0b111111111);
}
int evaluate(uint16_t X, uint16_t O)
{
    for (uint16_t pattern : winningpatterns)
    {
        if ((X & pattern) == pattern)
            return 1;
        else if ((O & pattern) == pattern)
            return -1;
    }
    return 0;
}
bool checkGameEnd(uint16_t X, uint16_t O)
{
    int eval = evaluate(X, O);
    if (eval == 1)
    {
        cout << "X wins";
        return true;
    }
    else if (eval == -1)
    {
        cout << "O wins";
        return true;
    }
    else if (noMoveLeft(X, O))
    {
        cout << "Draw";
        return true;
    }
    return false;
}
int minimax(uint16_t X, uint16_t O, int depth, char player, int alpha, int beta)
{
    int alphaOrig = alpha;
    int betaOrig = beta;
    uint32_t key = (X << 9) | O;
    position &entry = TT[key];
    if (entry.flag != EMPTY)
    {
        if (entry.flag == EXACT)
        {
            return entry.score;
        }
        else if (entry.flag == LOWERBOUND)
        {
            if (entry.score > alpha)
                alpha = entry.score;
        }
        else if (entry.flag == UPPERBOUND)
        {
            if (entry.score < beta)
                beta = entry.score;
        }

        if (alpha >= beta)
        {
            return entry.score;
        }
    }
    int score = evaluate(X, O);
    if (score == 1)
        return 10 - depth;
    else if (score == -1)
        return -10 + depth;
    if (noMoveLeft(X, O))
        return 0;
    uint16_t empty = ~(X | O) & 0b111111111;
    if (player == 'X')
    {
        score = -100;
        while (empty)
        {
            uint16_t move = empty & -empty;
            int eval = minimax(X | move, O, depth + 1, 'O', alpha, beta);
            if (eval > score)
                score = eval;
            if (eval > alpha)
                alpha = eval;
            if (alpha >= beta)
                break;
            empty = empty ^ move;
        }
    }
    else if (player == 'O')
    {
        score = 100;
        while (empty)
        {
            uint16_t move = empty & -empty;
            int eval = minimax(X, O | move, depth + 1, 'X', alpha, beta);
            if (eval < score)
                score = eval;
            if (eval < beta)
                beta = eval;
            if (alpha >= beta)
                break;
            empty = empty ^ move;
        }
    }
    entry.score = score;
    if (score <= alphaOrig)
        entry.flag = UPPERBOUND;
    else if (score >= betaOrig)
        entry.flag = LOWERBOUND;
    else
        entry.flag = EXACT;
    return score;
}
void makeMove(uint16_t &X, uint16_t &O, char player)
{
    uint16_t empty = ~(X | O) & 0b111111111;
    uint16_t bestmove = 0;
    int rand_count = 0;
    if (player == 'X')
    {
        int score = -100;
        while (empty)
        {
            uint16_t move = empty & -empty;
            int eval = minimax(X | move, O, 1, 'O', -100, 100);
            if (eval > score)
            {
                score = eval;
                bestmove = move;
            }
            else if (eval == score)
            {
                rand_count++;
                int r = rand() % rand_count;
                if (r == 0)
                    bestmove = move;
            }
            empty = empty ^ move;
        }
        X = (X | bestmove);
    }
    else if (player == 'O')
    {
        int score = 100;
        while (empty)
        {
            uint16_t move = empty & -empty;
            int eval = minimax(X, O | move, 1, 'X', -100, 100);
            if (eval < score)
            {
                score = eval;
                bestmove = move;
            }
            else if (eval == score)
            {
                rand_count++;
                int r = rand() % rand_count;
                if (r == 0)
                    bestmove = move;
            }
            empty = empty ^ move;
        }
        O = (O | bestmove);
    }
}
void displayBoard(uint16_t X, uint16_t O)
{
    uint16_t pointer = 0b100000000;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if ((X & pointer) == pointer)
                cout << "X ";
            else if ((O & pointer) == pointer)
                cout << "O ";
            else
                cout << "_ ";
            pointer = pointer >> 1;
        }
        cout << endl;
    }
}
int main()
{
    srand(time(0));
    uint16_t X = 0;
    uint16_t O = 0;
    char player, computer;
    displayBoard(X, O);
    cout << "Choose X or O" << endl;
    cin >> player;
    if (player == 'X')
    {
        computer = 'O';
    }
    else if (player == 'O')
    {
        computer = 'X';
        makeMove(X, O, computer);
        displayBoard(X, O);
    }
    else
    {
        cout << "Player Invalid: Choose X or O";
        return 0;
    }
    int eval = 0;
    int row = -1, col = -1;
    while (true)
    {
        cout << "Place '" << player << "' in row: column:" << endl;
        cin >> row >> col;
        if (row < 0 || row > 2 || col < 0 || col > 2)
        {
            cout << "Invalid move:Index out of bounds\n";
            continue;
        }
        int ind = row * 3 + col + 1;
        uint16_t pos = 0b1 << (9 - ind);
        if (((X | O) & pos) != 0)
        {
            cout << "Invalid move:Index is already occupied\n";
            continue;
        }
        if (player == 'X')
            X = X | pos;
        else if (player == 'O')
            O = O | pos;
        displayBoard(X, O);
        if (checkGameEnd(X, O))
            break;
        makeMove(X, O, computer);
        cout << "Computer Played: " << endl;
        displayBoard(X, O);
        if (checkGameEnd(X, O))
            break;
    }
    return 0;
}
