#pragma once

#include <memory.h>
#include <iostream>
#include <fstream>

const int num_of_pockets = 14;
const __int8 init_num_of_balls = 4;
const __int8 max_num_of_balls_in_pot = 40;
const __int8 _pockets_reset[num_of_pockets] = { 6, 6, 6, 6, 6, 6, 0, 6 , 6, 6, 6, 6, 6, 0 };
const __int8 _pockets_test1[num_of_pockets] = { 0, 0, 2, 1, 12, 12, 9, 0 , 13, 0, 13, 0, 1, 9 };



struct board
{
public:
	
	__int8 _pockets[num_of_pockets];
	void reset();
	bool move(__int8 pos, board &out);
	bool no_more_moves(short turn);
	int score();
	bool end();
	void prepare_cache();
	void validate_cache();
	void print();

};

