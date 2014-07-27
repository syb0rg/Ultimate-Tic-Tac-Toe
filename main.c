#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ROWS 9
#define COLS 9

typedef char Board[ROWS][COLS];
typedef char MetaBoard[ROWS / 3][COLS / 3];
typedef enum {VALID, NOT_A_DIGIT, NOT_IN_BOARD, SPACE_OCCUPIED, OUT_OF_BOUNDS} MoveStatus;
static const char marks[3] = {'O', 'X', '#'};

void fillSubBoard(Board board, int x, int y, char c);
int getBound(int in);
void printBoard(Board board);
static int checkMeta(MetaBoard meta);
static int checkBoard(Board board, MetaBoard meta, int player, int row, int column);
MoveStatus validCoords(Board board, int row, int column, int rowBound, int columnBound);

void fillSubBoard(Board board, int x, int y, char c)
{
	x -= (x % 3); // quickly set x to left bound of sub-board
	y -= (y % 3); // quickly set y to upper bound of sub-board
    for (int rowMax = x + 2, row = x; row <= rowMax; ++row)
    {
        for (int columnMax = y + 2, column = y; column <= columnMax; ++column)
        {
            board[row][column] = c;
        }
    }
}

int getBound(int in)
{
    return ((in < 9) ? in / 3 : -1);
}

void printBoard(Board board)
{
printf("\n=============||===========||=============\n");
	
    for (int row = 0; row < ROWS; ++row)
    {
        printf("||");
        for (int column = 0; column < COLS; ++column)
        {
            if ('-' == board[row][column]) printf("%d,%d|", row, column);
            else printf(" %c |", board[row][column]);
            if (0 == (column + 1) % 3) printf("|");
        }
        if (0 == (row + 1) % 3) printf("\n=============||===========||=============\n");
        else printf("\n-----|---|---||---|---|---||---|---|-----\n");
    }
}

static int checkMeta(MetaBoard meta)
{
    const int xStart[ROWS - 1] = {0,  0,  0,  0,  1,  2,  0,  0};
    const int yStart[COLS - 1] = {0,  1,  2,  0,  0,  0,  0,  2};
    const int xDelta[ROWS - 1] = {1,  1,  1,  0,  0,  0,  1,  1};
    const int yDelta[COLS - 1] = {0,  0,  0,  1,  1,  1,  1,  1};
    int startx = 0;
	int starty = 0;
	int deltax = 0;
	int deltay = 0;

    for (int trip = 0; trip < ROWS - 1; ++trip)
    {
        startx = xStart[trip];
        starty = yStart[trip];
        deltax = xDelta[trip];
        deltay = yDelta[trip];
        // main logic to check if a subboard has a winner
        if ('-' != meta[startx][starty] &&
            meta[startx][starty] == meta[startx + deltax][starty + deltay] &&
            meta[startx][starty] == meta[startx + deltax + deltax][starty + deltay + deltay]) return 1;
    }
    return 0;
}

static int checkBoard(Board board, MetaBoard meta, int player, int row, int column)
{
    const int xStart[ROWS - 1] = {0,  0,  0,  0,  1,  2,  0,  0};
    const int yStart[COLS - 1] = {0,  1,  2,  0,  0,  0,  0,  2};
    const int xDelta[ROWS - 1] = {1,  1,  1,  0,  0,  0,  1,  1};
    const int yDelta[COLS - 1] = {0,  0,  0,  1,  1,  1,  1,  1};
	int startx = 0;
	int starty = 0;
	int deltax = 0;
	int deltay = 0;
	int oCounter = 0;
	int xCounter = 0;
	int status = 0;

	row -= (row % 3); // quickly set x to left bound of sub-board
	column -= (column % 3); // quickly set y to upper bound of sub-board

	// check if board has already been won or is a tie
	for (int rowMax = row + 2, startx = row; startx <= rowMax; ++startx) // uses startx value to reduce the number of variables
    {
        for (int columnMax = column + 2, starty = column; starty <= columnMax; ++starty) // uses starty to reduce the number of variables
        {
            if(board[startx][starty] == marks[0]) ++oCounter;
			if(board[startx][starty] == marks[1]) ++xCounter;
			if(9 == oCounter || 9 == xCounter) status = 1; // make it so player can move anywhere
			if (9 == oCounter + xCounter)
			{
				fillSubBoard(board, row, column, marks[2]); // set the board to a neutral character
				meta[getBound(row)][getBound(column)] = marks[2]; // set the metaboard coords to a tied state
				return checkMeta(meta); // we don't need to check if board had been won, we just found it was a tie
			}
        }
    }

	// check if board has been won this round
    for (int trip = 0; trip < ROWS - 1; ++trip)
    {
        startx = row + xStart[trip];
        starty = column + yStart[trip];
        deltax = xDelta[trip];
        deltay = yDelta[trip];
        if ('-' != board[startx][starty] &&
            board[startx][starty] == board[startx + deltax][starty + deltay] &&
            board[startx][starty] == board[startx + deltax + deltax][starty + deltay + deltay])
        {
            fillSubBoard(board, row, column, marks[player]);
            meta[getBound(row)][getBound(column)] = marks[player];
        }
    }
    return (status + checkMeta(meta)); // always check if the game has a winner
}

MoveStatus validCoords(Board board, int row, int column, int rowBound, int columnBound)
{
    if (!isdigit((unsigned char)(((int)'0') + row)) && !isdigit((unsigned char)(((int)'0') + column))) return NOT_A_DIGIT; // supplied coords aren't digits 1-9
    else if (row > ROWS - 1 || column > COLS - 1) return NOT_IN_BOARD; // supplied coords aren't within the bounds of the board
    else if ('-' != board[row][column]) return SPACE_OCCUPIED; // supplied coords are occupied by another character
    else if (-1 == rowBound && -1 == columnBound) return VALID; // supplied coords can move anywhere
    else if (((row > rowBound * 3 + 2 || column > columnBound * 3 + 2) ||
              (row < rowBound * 3 || column < columnBound * 3)) &&
             (rowBound >= 0 && columnBound >= 0)) return OUT_OF_BOUNDS; // coords aren't within the sub-board specified by the previous move
    else return VALID; // didn't fail anywhere else, so coords are valid
}

int main(void)
{
    int winner = 0;
	int row = 0;
	int column = 0;
	int rowBound = -1; // set initially so player 1 can move anywhere
	int columnBound = -1; // set initially so player 1 can move anywhere
	int error = 0;
    char tempRow = '\0';
	char tempColumn = '\0';
    Board board;
    MetaBoard meta;
	static char const *errors[] =
	{
		0,
		"Invalid Input.",
		"Out of board's bounds.",
		"That space is already used.",
		"Your move was in the wrong sub-board."
	};

    // initialize boards and fill with '-'
    memset(board, '-', ROWS * COLS);
    memset(meta, '-', (ROWS / 3) * (COLS / 3));

    // game loop
    for (int turn = 0; turn < ROWS * COLS && !winner; ++turn)
    {
        int player = turn % 2;
        printBoard(board);
        printf("Player %d, enter the coordinates (x, y) to place %c: ", player + 1, (1 == player) ? 'X' : 'O');
        do
        {
            scanf("%c, %c", &tempRow, &tempColumn);
            while (getchar() != '\n'); // pick up superfluous input so we don't run into problems when we scan for input again
            row = abs((int) tempRow - '0');
            column = abs((int) tempColumn - '0');
			if(0 != (error = validCoords(board, row, column, rowBound, columnBound))) printf("%s Re-enter:", errors[error]);
        } while (error);

        board[row][column] = marks[player];
        switch(checkBoard(board, meta, player, row, column))
        {
            case 1:
                // next move can be anywhere
                rowBound = -1;
                columnBound = -1;
                break;
            case 2:
                winner = player + 1;
                break;
            default:
                rowBound = row % 3;
                columnBound = column % 3;
                break;
        }
    }
    printBoard(board);

    if(!winner) printf("The game is a draw\n");
    else printf("Player %d has won\n", winner);
}