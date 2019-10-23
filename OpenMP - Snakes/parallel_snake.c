#include "parallel_snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

void snakes_to_entireSnakes(int** world, int num_snakes, struct snake *snakes, NewSnake *eSnakes, int num_lines, int num_cols)
{
	struct coord head;
	int i, j, ncUp, ncDown, ncLeft, ncRight, more = 0;					//neighbour coordinates left,right,up,down
	int ln,cl;
	#pragma omp parallel for
	for (i = 0; i < num_snakes; i++)
	{
		eSnakes[i].encoding = snakes[i].encoding;
		eSnakes[i].direction = snakes[i].direction;
		eSnakes[i].vectorSize = 1;
		eSnakes[i].snakeVector = (struct coord *)calloc(1, sizeof(struct coord)); 
		eSnakes[i].snakeVector[0].line = snakes[i].head.line;
		eSnakes[i].snakeVector[0].col = snakes[i].head.col;
	}
	for (i = 0; i < num_snakes; i++)								//sterg toate capetele serpilor de pe harta
	{
		ln = snakes[i].head.line;
		cl = snakes[i].head.col;
		world[ln][cl] = 0;
	}
	#pragma omp parallel for private(j, head, ncUp, ncLeft, ncRight, ncDown, more)
	for (i = 0; i < num_snakes; i++)
	{
		for(j = 1;j > 0; j++)
		{
			more = 0;												//daca mai am de citit segmente ale sarpelui, more va fi 1
			head.line = eSnakes[i].snakeVector[j-1].line;
			head.col = eSnakes[i].snakeVector[j-1].col;
			ncUp = (head.line - 1 + num_lines) % num_lines;			//punctele vecine ale "capului sarpelui"
			ncLeft = (head.col - 1 + num_lines) % num_lines;		//avand in vedere ca mereu sterg capul, apoi ii caut vecinii s.a.m.d
			ncRight = (head.col + 1) % num_lines;					//ultimul segment citit din sarpe va fi considerat "capul" sarpelui
			ncDown = (head.line + 1) % num_lines;

			if(world[ncUp][head.col] == snakes[i].encoding)
			{
				head.line = ncUp;
				more = 1;
			}
			else if(world[head.line][ncLeft] == snakes[i].encoding)
			{
				head.col = ncLeft;
				more = 1;
			}
			else if(world[head.line][ncRight] == snakes[i].encoding)
			{
				head.col = ncRight;
				more = 1;		
			}
			else if(world[ncDown][head.col] == snakes[i].encoding)
			{
				head.line = ncDown;
				more = 1;
			}
			if(more == 0)
				break;
			else
			{	
				if(j >= eSnakes[i].vectorSize)					//realocare, in caz ca sarpele are mai mule segmente decat are alocat vectorul
				{
					eSnakes[i].vectorSize = eSnakes[i].vectorSize * 2;
		  			eSnakes[i].snakeVector = (struct coord *)realloc(eSnakes[i].snakeVector,eSnakes[i].vectorSize* sizeof(struct coord));
				}
			}		
			eSnakes[i].snakeVector[j].line = head.line;
			eSnakes[i].snakeVector[j].col = head.col;
			eSnakes[i].snakeSize = j+1;
			world[head.line][head.col] = 0;
		}
	}
	for (i = 0; i < num_snakes; i++)						//rescriu serpii pe harta
	{
		for(j = 0; j < eSnakes[i].snakeSize; j++)
		{
			ln = eSnakes[i].snakeVector[j].line;
			cl = eSnakes[i].snakeVector[j].col;
			world[ln][cl] = eSnakes[i].encoding;
		}
	}
}

void get_coords(int initLine, int initCol, char direction,int* line,int* col, int num_lines)
{
	if(direction == 'N')
	{
		(*line) = (initLine - 1 + num_lines) % num_lines;
		(*col) = initCol;
	}
	else if(direction == 'S')
	{
		(*line) = (initLine + 1) % num_lines;
		(*col) = initCol;
	}
	else if(direction == 'V')
	{
		(*col) = (initCol - 1 + num_lines) % num_lines;
		(*line) = initLine;

	}
	else if(direction == 'E')
	{
		(*col) = (initCol + 1) % num_lines;
		(*line) = initLine;
	}
	return;
}


void run_simulation(int num_lines, int num_cols, int **world, int num_snakes,
	struct snake *snakes, int step_count, char *file_name) 
{
	// TODO: Implement Parallel Snake simulation using the default (env. OMP_NUM_THREADS) 
	// number of threads.
	//
	// DO NOT include any I/O stuff here, but make sure that world and snakes
	// parameters are updated as required for the final state.
	NewSnake* eSnakes;
	struct coord* auxCoords;
	int i,j,k,ln = 0 , cl = 0, collision = 0, size = 0, thrds = 0;
	eSnakes = (NewSnake *)calloc(num_snakes, sizeof(NewSnake));
	auxCoords = (struct coord *)calloc(num_snakes,sizeof(struct coord));

	thrds = atoi(getenv("OMP_NUM_THREADS"));
	omp_set_num_threads(thrds);

	snakes_to_entireSnakes(world, num_snakes,snakes, eSnakes, num_lines, num_cols);
	
	for(j = 0; j < step_count; j++)
	{
		#pragma omp parallel for private(size,cl,ln)
		for(i = 0; i < num_snakes; i ++)															//se sterg cozile serpilor
		{
			size = eSnakes[i].snakeSize;
			world[eSnakes[i].snakeVector[size - 1].line][eSnakes[i].snakeVector[size - 1].col] = 0;
			get_coords(eSnakes[i].snakeVector[0].line, eSnakes[i].snakeVector[0].col, eSnakes[i].direction, &ln, &cl,num_lines);	//noile capete ale serpilor
			auxCoords[i].col = cl;
			auxCoords[i].line = ln;
		}

		#pragma omp parallel for private(k)
		for(i = 0; i < num_snakes; i ++)
		{
			if(world[auxCoords[i].line][auxCoords[i].col] != 0)										//se verifica coliziunea
			{
				collision = 1;
				i = num_snakes + 10;
			}
			else
			{
				
				for(k = 0;k < num_snakes;k++)
				{
					if(i!= k && auxCoords[i].col == auxCoords[k].col && auxCoords[i].line == auxCoords[k].line)
					{
						collision = 1;
						//break;
					}	
				}
			}
		}
		if(collision == 1)																				//daca a avut loc coliziune
		{
			for(i = 0; i < num_snakes; i ++)															
			{
				size = eSnakes[i].snakeSize;															//se scriu pe harta vechile cozi ale serpilor
				world[eSnakes[i].snakeVector[size - 1].line][eSnakes[i].snakeVector[size - 1].col] = eSnakes[i].encoding;
			}
			return;
		}

		#pragma omp parallel for
		for(i = 0; i < num_snakes; i ++)											//	noul cap=[0*]		[0]	[1] [2] [3] [4]												
		{																	  	 	//                       \	 \	 \	 \	 \	  
																					//					 [0*] [1] [2] [3] [4] \ => vechea coada se pierde						
			world[auxCoords[i].line][auxCoords[i].col] = eSnakes[i].encoding;														
			memmove(&(eSnakes[i].snakeVector[1]), &(eSnakes[i].snakeVector[0]) , (eSnakes[i].snakeSize - 1)* sizeof(struct coord));
			eSnakes[i].snakeVector[0].line = auxCoords[i].line;											//sunt scrise noile capete in vectorul fiecarui sarpe
			eSnakes[i].snakeVector[0].col = auxCoords[i].col;
			snakes[i].head.line = auxCoords[i].line;
			snakes[i].head.col = auxCoords[i].col;
	
		}
	}
	
	
}