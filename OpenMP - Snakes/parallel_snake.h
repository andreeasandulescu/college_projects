#ifndef __PARALLEL_SNAKE_H__
#define __PARALLEL_SNAKE_H__

#include "main.h"

typedef struct entireSnake
{
	struct coord *snakeVector;
	int vectorSize;
	int snakeSize;
	int encoding;
	char direction;
}NewSnake;

void snakes_to_entireSnakes(int** world, int num_snakes, struct snake *snakes, NewSnake *eSnakes,int num_lines, int num_cols);
void get_coords(int initLine, int initCol, char direction,int* line,int* col, int num_lines);

#endif