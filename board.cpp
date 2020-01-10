#include "board.h"
#include <iomanip>

__int8 _pockets_move_cache[num_of_pockets * num_of_pockets * max_num_of_balls_in_pot]; // easly fit in L1 cache
__int8 _pockets_last_pos_cache[num_of_pockets * max_num_of_balls_in_pot]; // easly fit in L1 cache

void board::reset()
{
	memcpy((void*)_pockets, (void*)_pockets_reset, num_of_pockets);
	//memcpy((void*)_pockets, (void*)_pockets_test1, num_of_pockets);
}

bool board::move(__int8 pos, board &bout)
{
	__int8 n = _pockets[pos];

	if (n == 0)
		return false;

	
	__int8 pos_save = pos;
	bool side  =pos < 6;
	bool new_side = side;

	bout = *this;
	bool first_round = true;

	while ((n > 0 && first_round || n>1) && side == new_side && pos != 13 && pos != 6)
	{

		__int16 next_move = (n * num_of_pockets + pos * num_of_pockets * max_num_of_balls_in_pot); // take move from cache
		__int16 last_pot_move = (n + pos * max_num_of_balls_in_pot); // take move from cache
		__int8* pCache = &_pockets_move_cache[next_move];
			
		for (int i = 0; i < num_of_pockets; ++i)
			bout._pockets[i] = pCache[i] + bout._pockets[i];

		//next pos will be :

		pos = _pockets_last_pos_cache[last_pot_move]; // (pos + n) % (num_of_pockets - 1);
		n = bout._pockets[pos];
		new_side = pos < 6;
		first_round = false;
	}

	// if landed on own empty spot take opsite pos for yourself 

	if (n == 1 && new_side == side && pos != 13 && pos != 6)
	{
		__int8 take_balls = 12 - pos;
		bout._pockets[side?6:13] += bout._pockets[take_balls] + 1;
		bout._pockets[take_balls] = 0;
		bout._pockets[pos] = 0;


	}
	int sum = 0;
	for (int i = 0; i < 14; i++)
	{
		sum += bout._pockets[i];
	}
	if (sum != 72)
		int gfd = 54646;

	return true;
}
void board::prepare_cache()
{
	for (__int8 pos = 0; pos < 13; pos++)
	{

		if (pos == 6)
			continue;

		for (__int8 n = 0; n < max_num_of_balls_in_pot; n++) // n is how manny balls in the pot we want to move
		{
			// fill one move

			__int8 d = n / (num_of_pockets - 1);
			__int8 r = n % (num_of_pockets - 1);
			__int16 next_move = (n * num_of_pockets + pos * num_of_pockets * max_num_of_balls_in_pot);
			__int16 move_end = (n  + pos * max_num_of_balls_in_pot);
			_pockets_move_cache[next_move + pos] = d - n;
			int dr = r;

			if (dr==0)
				_pockets_last_pos_cache[move_end] = pos;
			for (int i = 1; i < num_of_pockets; i++)
			{
				__int8 cyclic = ((i + pos) % (num_of_pockets));

				if (cyclic == 6 && pos > 6)
					_pockets_move_cache[next_move + 6] = 0;
				else if (cyclic == 13 && pos < 6)
					_pockets_move_cache[next_move + 13] = 0;
				else if (dr > 0)
				{
					_pockets_move_cache[next_move + cyclic] = d + 1;
					dr--;
					if (dr == 0)
						_pockets_last_pos_cache[move_end] = cyclic;
				}
				else
					_pockets_move_cache[next_move + cyclic] = d;

			}

		}
	}


}
void board::validate_cache()
{
	std::ofstream f("log_cache.txt");
	for (__int8 pos = 0; pos < 13; pos++)
	{
		if (pos == 6)
			continue;
		for (__int8 n = 0; n < max_num_of_balls_in_pot; n++) // n is how manny balls in the pot we want to move
		{
			// fill one move
			__int16 next_move = (n * num_of_pockets + pos * num_of_pockets * max_num_of_balls_in_pot);
			__int16 last_move = (n + pos * max_num_of_balls_in_pot);

			for (size_t i = 0; i < num_of_pockets; i++)
				f << (int)_pockets_move_cache[next_move + i] << ',';
			f << "last move = " << (int)_pockets_last_pos_cache[last_move];
			f << '\n';

		}
		f << '\n';
	}


}

bool board::end()
{
	return  (_pockets[6] > 36 || _pockets[13]> 36);
		
}

bool board::no_more_moves(short turn)
{
	int sum = 0;

	for (int i=7*turn; i < 6+7*turn;  i++)
		sum += _pockets[i];
	return sum == 0;
}

int board::score()
{
	int score = (short)_pockets[6]-(short)_pockets[13];
	//if (end())
	//	std::cout << "score on end game ==  " << score << std::endl;
	return score;
}

void board::print()
{
	std::cout << std::setw(3);

	for (int i = 12; i >6; i--)
		std::cout << std::setw(3) << (int)_pockets[i];
	std::cout << std::endl;
	

	std::cout << (int)_pockets[13] << "                   " << (int)_pockets[6] << std::endl;

	for (int i = 0; i < 6; i++)
		std::cout << std::setw(3) << (int)_pockets[i];

	std::cout << std::endl;

}