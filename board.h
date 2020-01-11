#pragma once

#include <memory.h>
#include <iostream>
#include <fstream>

const char num_of_pockets = 14;
const char init_num_of_balls = 4;
const char max_num_of_balls_in_pot = 40;
const char _pockets_reset[num_of_pockets] = { 6, 6, 6, 6, 6, 6, 0, 6 , 6, 6, 6, 6, 6, 0 };
const char _pockets_test1[num_of_pockets] = { 0, 0, 2, 1, 12, 12, 9, 0 , 13, 0, 13, 0, 1, 9 };



struct board
{
public:
	
	char _pockets[num_of_pockets];
	void reset();
	bool move(char pos, board &out);
	bool no_more_moves(short turn);
	int score();
	bool end();
	void prepare_cache();
	void validate_cache();
	void print();

};

