#include "computer_agent.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/*
 * In this file, implement your AI agent.
 *
 * Use the minimax algorithm to determine the next move.
 *
 * Look at player_agent.h and player_agent.c for an example of how to
 * implement each function.
 */

int maxgenerate(board_t * b, player_t player, unsigned int * best, player_t computer);

int mingenerate(board_t * b, player_t player, unsigned int * best, player_t computer)
{
	int score = 1000;
	unsigned int best_move = 0;
	// how to refer the winmove to the person is the question.

	unsigned int width = board_get_width(b);

	for(unsigned int i=0; i < width; ++i)
	{
		// I need to replace this with board_can_play 
		if(!board_can_play_move(b, player, i))
			continue;

		board_play(b, i, player);
		player_t winner = PLAYER_EMPTY;
		if(board_has_winner(b, &winner)!=0)
		{
			int tmp_score = 1000;
			if(winner == computer)
			{
				tmp_score = 100;
			}
			else if(winner != PLAYER_EMPTY)
			{
				tmp_score = -100;
			}
			else
			{
				tmp_score = 0;
			}
			
			if(score > tmp_score) // it is sketchy on player here
			{
				score = tmp_score;
				best_move = i;
				
			}
		}
		else
		{
			
			player_t new_player = PLAYER_EMPTY;
			if(player == PLAYER_YELLOW)
				new_player = PLAYER_BLUE;
			else if(player == PLAYER_BLUE)
				new_player = PLAYER_YELLOW;
			
			int tmp_score_two = maxgenerate(b, new_player, best, computer);
			if(score > tmp_score_two)
			{
				score = tmp_score_two;
				best_move = i;
			}
		}
		// we need to return the score and the line here
		board_unplay(b, i);
		if(score == -100)
		{
			break;
		}
	}
	*best = best_move;
	return score;
}

typedef struct
{
   char * name;
} computer_agent_data_t;

static
const char * computer_agent_describe (agent_t * gthis)
{
   computer_agent_data_t * data = (computer_agent_data_t *) gthis->data;
   return data->name;
}

static
int computer_agent_play (agent_t * gthis, const board_t * b,
        player_t color, unsigned int * move)
{
   while(true)
   {
      board_t * new_b;
      board_duplicate(&new_b, b);
   
      maxgenerate(new_b, color, move, color);
      board_destroy(new_b);
      if(!board_can_play_move(b,color, *move))
	  {
	     continue;
	  }
      
      return 0;
   }
}

int maxgenerate(board_t * b, player_t player, unsigned int * best, player_t computer)
{
	int score = -1000;
	unsigned int best_move = 0;
	// how to refer the winmove to the person is the question.

	unsigned int width = board_get_width(b);

	for(unsigned int i=0; i < width; ++i)
	{
		// I need to replace this with board_can_play 
		if(!board_can_play_move(b, player, i))
			continue;

		board_play(b, i, player);
		player_t winner = PLAYER_EMPTY;
		if(board_has_winner(b, &winner)!=0)
		{
			int tmp_score = -1000;
			if(winner == computer)
			{
				tmp_score = 100;
			}
			else if(winner != PLAYER_EMPTY)
			{
				tmp_score = -100;
			}
			else
			{
				tmp_score = 0;
			}
			
			if(score < tmp_score) // it is sketchy on player here
			{
				score = tmp_score;
				best_move = i;
			}
		}
		else
		{
			
			player_t new_player = PLAYER_EMPTY;
			if(player == PLAYER_YELLOW)
				new_player = PLAYER_BLUE;
			else if(player == PLAYER_BLUE)
				new_player = PLAYER_YELLOW;

			int tmp_score_two = mingenerate(b, new_player, best, computer);
			if(score < tmp_score_two)
			{
				score = tmp_score_two;
				best_move = i;
			}
		}
		// we need to return the score and the line here
		board_unplay(b, i);
		if(score == 100)
		{
			break;
		}
	}
	*best = best_move;
	return score;
}

static
bool computer_agent_destroy (agent_t * this)
{
   computer_agent_data_t * data = (computer_agent_data_t *) this->data;
   free (data->name);
   free (this->data);
   return true;
}

agent_t * computer_agent_create (const char * name)
{
    // Allocate vtable structure
	char * name_two = malloc(strlen("computer agent")+1);
	strcpy(name_two, "computer agent");

    agent_t * n = malloc (sizeof(agent_t));

    n->destroy = computer_agent_destroy;
    n->play = computer_agent_play;
    n->describe = computer_agent_describe;

    computer_agent_data_t * data = malloc (sizeof(computer_agent_data_t));
    n->data = data;

	data->name = name_two;
    return n;
}
