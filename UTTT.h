#ifndef UTTT_H
#define UTTT_H

#define ROWS 9
#define COLS 9

typedef char Board[ROWS][COLS];
typedef char MetaBoard[ROWS / 3][COLS / 3];
typedef enum {VALID, NOT_A_DIGIT, NOT_IN_BOARD, SPACE_OCCUPIED, OUT_OF_BOUNDS} MoveStatus;
static const char marks[] = {'O', 'X'};

void fillSubBoard(Board board, int x, int y, char c);
int getBound(int in);
void printBoard(Board board);
static int checkMeta(MetaBoard meta);
static int checkBoard(Board board, MetaBoard meta, int player, int row, int column);
MoveStatus validCoords(Board board, int row, int column, int rowBound, int columnBound);

#endif // UTTT_H