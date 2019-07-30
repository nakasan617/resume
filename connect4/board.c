#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

/**
 * In this file, implement all of the functions for board.h
 *
 * Once done, make sure you pass all the tests in board_test.c
 * (make test_board)
 *
 */

struct board_t {
	unsigned int height;
	unsigned int width;
	unsigned int run;
	// we may need more but I will keep it simple
	unsigned char * board_sit;
};

/// Create a new empty board of the given game configuration.
///    width  = width of the board
///    height = height of the board
///    run    = number of pieces in a row to consider a win
///
/// Returns a new board pointer in *b
/// 
/// If initial is not NULL, then it must point to a (one-dimensional)
/// array of width*height player_t elements containing the initial
/// board configuration, ROW BY ROW, starting with the bottom ROW
/// (i.e. lowest row number, bottom of board),
/// LEFT-TO-RIGHT (i.e. incrementing column numbers).
/// 
/// The board is numbered as follows:
///
/// ROW
///  ...
///  3
///  2
///  1
///  0
///    0 1 2 3 4 COLUMN
/// 
/// In other words, the first row a piece is placed in is row 0
/// And the lower left corner of the board is row 0, column 0
///
/// When initial == NULL, the board should be initialized to be empty.
bool board_create (board_t ** b, unsigned int height,
        unsigned int width, unsigned int run,
        const player_t * initial)
{
	if(b == NULL)
	{
		return false;
	}

	// I malloced twice here
	*b = malloc(sizeof(struct board_t));
	(*b)->height = height;
	(*b)->width = width;
	(*b)->run = run;
	(*b)->board_sit = malloc(height*width*sizeof(player_t)); 

	// player_t is unsigned char
	if(initial == NULL)
	{	
		memset((*b)->board_sit, PLAYER_EMPTY, height*width*sizeof(player_t));
	}
	else
	{
		memcpy((*b)->board_sit, initial, height*width*sizeof(player_t));
	}

	return true;

}


/// Destroy the given board
bool board_destroy (board_t * b)
{
	if(b->board_sit != NULL)
	{
		free(b->board_sit);	
	}
    //printf("are you okay till here?\n");	
	free(b);
	return true;
}

/// Return the height of the board
unsigned int board_get_height (const board_t * b)
{
	return b->height;
}

/// Return the width of the board
unsigned int board_get_width (const board_t * b)
{
	return b->width;
}

/// Return how many pieces need to be in a straight line before the game 
/// is considered to be won. (straight line: horizontal, diagonal or vertical)
unsigned int board_get_run (const board_t * b)
{
	return b->run;
}

/// Return true if the given move is valid, false otherwise
/// If true, the board was changed and the move was made.
/// If false, the board remains the same.
bool board_play (board_t * b, unsigned int column, player_t player)
{
	if(board_can_play_move(b, player, column) == false)
	{
		return false;
	}

	int j = b->width*(b->height - 1) + column;

	for(int i = 0; i < (b->height); i++)
	{
		if((b->board_sit)[j] == PLAYER_EMPTY)
		{
			if( 0 <= j && j < (b->width))
			{
				(b->board_sit)[j] = player;
				return true;
			}
			else
			{
				j = j-(b->width);
			}
		
		}
		else
		{
			(b->board_sit)[j+(b->width)] = player;
			return true;
		}
	}

	return false;
}

/// Return true if the specified player can make a move on the
/// board. THERE IS NO NEED TO KEEP TRACK OF THE TURN OR TO VALIDATE THIS.
/// The board is not changed.
bool board_can_play (const board_t * b, player_t p)
{
	int flag = 1;
	for(int i = 0; i < b->width; i++)
	{
		if((b->board_sit)[((b->height)-1)*(b->width)+i] == PLAYER_EMPTY)
		{
			flag = 0;
		}
	}

	if(flag == 0)
		return true;
	else if (flag == 1)
		return false;
	return true;	
}

/// Return true if the specified move is valid
/// The board is not changed.
bool board_can_play_move (const board_t * b, player_t p, unsigned int
      column)
{
	if(0 > column || ((b->width)-1) < column)
	{
		return false;
	}	
	
	unsigned int concern = (b->width)*(b->height - 1) + column;

	if((b->board_sit)[concern] != PLAYER_EMPTY)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

/// Undo a move (i.e. make the specified board slot empty)
/// Return true if OK, false on error
/// If the function returns true, the board has been modified.
/// If the function returns false, the board has not been modified.
/// There is no need to keep track of the which player last played.
bool board_unplay (board_t * b, unsigned int column)
{
	int j = b->width * (b->height - 1) + column;
	for(int i = 0; i < (b->height); i++)
	{
		if((b->board_sit)[j] == PLAYER_EMPTY)
		{
			j = j - b->width;
		}
		else 
		{
			b->board_sit[j] = PLAYER_EMPTY;
			return true;
		}
	}
	return false;
}

/// Duplicate a board
//      return true on success; New board needs to be freed 
///     with board_destroy.
///
/// Return true on success, false on error
bool board_duplicate (board_t ** newboard, const board_t * old)
{
	*newboard = malloc(sizeof(struct board_t));
	(*newboard)->height = old->height;
	(*newboard)->width = old->width;
	(*newboard)->run = old->run;
	(*newboard)->board_sit = malloc((old -> height)*(old -> width));
	memcpy((*newboard)->board_sit, old->board_sit, (old->height)*(old->width));
	return true;
	 
}

// helper function 
// this sees whether the position is valid or not
bool validfromvertical(const board_t * b, unsigned int pos)
{
	if(pos + (b->width)*(b->run-1) < (b->width)*(b->height))
		return true;
	else
		return false;
}

// helper function 
// this sees whether there is a run in certain position 
bool winpos_fromvertical(const board_t * b, unsigned int pos, player_t player)
{
	if(!validfromvertical(b, pos))
		return false;

	for(int i = 0; i < b->run; i++)
	{
		if(b->board_sit[pos+i*(b->width)] == player)
		{
			continue;
		}
		else;
			return false;
	}
	return true;
}

// helper function 
// this sees whether there is a run in a certain position 
bool wincolumn_fromvertical(const board_t * b, unsigned int column, player_t player)
{
	unsigned int pos = column;
	while(validfromvertical(b, pos))
	{
		if(winpos_fromvertical(b, pos, player))
		{
			//printf("this is winning pos %u\n", pos);
			return true;
		}

		pos = pos + b->width;
	}
	return false;
}

bool validfromhorizontal(const board_t * b, unsigned int pos)
{
	if(b->run > b->width)
		return false;
	if(pos%(b->width) >= (pos+(b->run-1))%(b->width))
		return false;
	else if(pos+(b->run-1) >= (b->width)*(b->height))
		return false;
	else 
		return true;
}

bool winpos_fromhorizontal(const board_t * b, unsigned int pos, player_t player)
{
	if(!validfromhorizontal(b, pos))
		return false;

	for(int i = 0; i < b->run; i++)
	{
		
		if(b->board_sit[pos+i] == player)
			continue;
		else 
			return false;
	}
	return true;
}

bool winrow_fromhorizontal(const board_t * b, unsigned int row, player_t player)
{
	unsigned int pos = row*(b->width);
	while(validfromhorizontal(b, pos))
	{
		if(winpos_fromhorizontal(b, pos, player))
		{
			//printf("%u\n",pos);
			return true;
		}

		pos = pos + 1;
	}
	return false;
}

bool validfromdiagonalup(const board_t * b, unsigned int pos)
{
	if(b->run > b->width || b-> run > b->height)
		return false;
	if(pos >= (b->width)*(b->height))
		return false;
	if((pos+(b->run - 1)*(b->width + 1))%(b->width) <= pos%(b->width))
		return false;

	if(pos+(b->run - 1)*(b->width) + (b->run - 1) >= (b->width)*(b->height))
		return false;
	
	return true;
}

bool winpos_fromdiagonalup(const board_t * b, unsigned int pos, player_t player)
{
	if(!validfromdiagonalup(b, pos))
		return false;
	for(int i = 0; i < b->run; i++)
	{
		if(b->board_sit[pos+i+i*b->width] == player)
		{
			//printf(" this is the positions to be changed %u\n", pos+i+i*b->width);
			continue;
		}
		else
			return false;
	}
	return true;
}

bool win_fromdiagonalup(const board_t * b, player_t player)
{

	for(unsigned int pos = 0; pos < b->width*b->height; pos++)
	{
		if(!validfromdiagonalup(b, pos))
			continue;
		if(winpos_fromdiagonalup(b, pos, player))
		{
			//printf("%u\n", pos);
			return true;
		}
		else
			continue;
	}
	
	return false;
}

bool validfromdiagonaldown(const board_t * b, unsigned int pos)
{
	if(b->run > b->width || b->run > b->height)
		return false;
	if(pos >= (b->width)*(b->height))
		return false;
	if(0 > pos - (b->width+1)*(b->run-1))
		return false;
	if(pos - (b->width-1)*(b->run-1) >= (b->width)*(b->height))
		return false;
	if((pos - (b->width-1)*(b->run-1)) % (b->width) <= pos % (b->width))
	{
		return false;
	}
	return true;
	
}
/*
bool validcurrentpos(const board_t * b, unsigned int pos, int i)
{
	if(pos >= b->width*b->height)
		return fals;
	if(i != 0 && 
}
*/
bool winpos_fromdiagonaldown(const board_t * b, unsigned int pos, player_t player)
{
	if(!validfromdiagonaldown(b, pos))
		return false;
	for(int i = 0; i < b->run; i++)
	{
		//printf("%u\n",pos);
		//printf("%u\n",pos-i*(b->width-1));
		if(pos-i*(b->width-1) >= (b->width)*(b->height))
			return false;
		
		if(b->board_sit[pos-i*(b->width-1)] == player)
		{
			continue;
		}
		else
			return false;
	}
	return true;
}

bool win_fromdiagonaldown(const board_t * b, player_t player)
{
	for(unsigned int pos = 0; pos < b->width*b->height; pos++)
	{
			
		if(!validfromdiagonaldown(b, pos))
			continue;
		if(winpos_fromdiagonaldown(b, pos, player))
		{
			//printf("this is the position I need to look at %u\n", pos);
			return true;
		}
		else 
			continue;
	}
	
	return false;
}
/// Return -1 if tie, 1 if winner, 0 if no winner
/// If there is a winner, set player to the winning color.
///
/// no winner means that the game can still continue (i.e. open positions)
/// winner means somebody has N tokens in a straight line.
/// tie means there are no more valid moves and there is no winner.
///
/// You can assume that only one of the above conditions is true.
///
int board_has_winner (const board_t * b, player_t * player)
{
	for(unsigned int column = 0; column < b->width; column++)
	{
		if(wincolumn_fromvertical(b, column, PLAYER_YELLOW))
		{
			*player = PLAYER_YELLOW;
			//printf("this is the column %u\n", column);
			return 1;
		}

		if(wincolumn_fromvertical(b, column, PLAYER_BLUE))
		{
			*player = PLAYER_BLUE;
			return 1;
		}
	}

	for(unsigned int row = 0; row < b->height; row++)
	{
		if(winrow_fromhorizontal(b, row, PLAYER_YELLOW))
		{
			*player = PLAYER_YELLOW;
			return 1;
		}
		
		if(winrow_fromhorizontal(b, row, PLAYER_BLUE))
		{
			*player = PLAYER_BLUE;
			return 1;
		}
	}

	if(win_fromdiagonalup(b, PLAYER_YELLOW))
	{
		*player = PLAYER_YELLOW;
		return 1;
	}	
	
	if(win_fromdiagonalup(b, PLAYER_BLUE))
	{
		*player = PLAYER_BLUE;
		return 1;
	}

	if(win_fromdiagonaldown(b, PLAYER_YELLOW))
	{
		*player = PLAYER_YELLOW;
		return 1;
	}
	
	if(win_fromdiagonaldown(b, PLAYER_BLUE))
	{
		*player = PLAYER_BLUE;
		return 1;
	}

	if(!board_can_play(b, PLAYER_YELLOW) && !board_can_play(b, PLAYER_BLUE))
		return -1;

	return 0;
}

/// Return game piece at specified position
/// Returns false if the position is not valid on the board,
/// returns true otherwise and copies the piece at that location on the board
/// to *piece
bool board_get (const board_t * b, unsigned int row, unsigned int column,
      player_t * piece)
{
	if(b->height <= row)
	{
		return false;
	}
	
	if(b->width <= column)
	{
		return false;
	}

	//printf("this is where it is error\n");
	unsigned int concern = b->width * row + column;

	*piece = b->board_sit[concern];

	return true;
}


/// Clear the board
/// Return true if success, false otherwise
bool board_clear (board_t * b)
{
	memset(b->board_sit, PLAYER_EMPTY, ((b->height)*(b->width)*sizeof(player_t)));
	return true;
}

/// Write the board to the given file (for visual aid for the player)
/// Return true if OK, false on error
/*
bool board_print (const board_t * b, FILE * f)
{
	if(f == NULL || b == NULL)
		return false;

	for(int i = 0; i< (b->width) * (b->height); i++)
	{
		switch ((b->board_sit)[i])
    	{
    		case PLAYER_EMPTY:
        		fprintf(f, ". ");
				break;
        	case PLAYER_YELLOW:
            	fprintf(f, "x ");
				break;
        	case PLAYER_BLUE:
            	fprintf(f, "o ");
				break;
    	}
	}
	fprintf(f, "\n");
	return true;
}
*/
bool board_print (const board_t * b, FILE * f)
{
	if(f == NULL || b == NULL)
		return false;

	for(int i = b->height -1; i>=0; i--)
	{
		for(int j = 0; j< b->width; j++)
		{ 
			int index = i*b->width + j;

			switch ((b->board_sit)[index])
    		{
    			case PLAYER_EMPTY:
        			fprintf(f, ". ");
					break;
        		case PLAYER_YELLOW:
            		fprintf(f, "x ");
					break;
        		case PLAYER_BLUE:
            		fprintf(f, "o ");
					break;
			}
    	}
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
	return true;
}
// this is to debug 
/*
bool board_print (const board_t * b, FILE * f)
{
	char translate [80];
	for(int i = 0; i < (b->width)*(b->height); i++)
	{
		if((b->board_sit)[i] == PLAYER_EMPTY)
		{
			translate[i] = 'b';
		}
		else if((b->board_sit)[i] == PLAYER_YELLOW)
		{
			translate[i] = 'x';
		} 
		else
		{
			translate[i] = 'o';
		}
		//fprintf(f,translate[i]);
		//fprintf(f, ',');
	}
	printf("%s\n", translate);
	return true;
}
*/
