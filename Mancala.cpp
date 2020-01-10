// Mancala.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "board.h"
#include <algorithm>
#include <vector>
#include <future>
#include <atomic>
#include <chrono>
#include <map>
#include <ctime>    


using namespace std;
std::atomic<int> threadcount;
std::atomic<__int64> op_count;
std::map<int, std::vector<int>> moves_map;
std::vector<int> scores_map;
const int max_level = 7;

std::ofstream out_moves("out_moves-.txt");

std::mutex gMutex;

int think(board b, int level, short turn)
{
	
	if (b.end() || level == max_level)
	{
		int score_end  = b.score();
		//std::lock_guard<std::mutex> lg(gMutex);
		//scores_map.push_back(score_end);
		return score_end;
	}
	if (b.no_more_moves(turn))
		return think(b, level+1, 1 - turn);

	int max_score = -99999;
	int score = 0;
	int max_pos = -1;
	board temp_move;
	

	std::vector<std::pair<std::future<int>,int>> futures;
	for (int i = 7*turn; i < 6+7*turn; i++)
	{
		if (b.move(i, temp_move))
		{
			{
				//std::lock_guard<std::mutex> lg(gMutex);
				//moves_map[level].push_back(i);
			}
			
			if (threadcount.load() < 3)
			{
				threadcount++;
				futures.push_back(std::make_pair(std::async(std::launch::async, think, temp_move, level + 1, 1 - turn), i));
			}
			else
			{
				
				score = think(temp_move, level + 1, 1 - turn);
				if (max_score < score)
				{
					max_score = score;
					max_pos = i;
				}
			}
		}
	}
	for (auto &fu : futures)
	{
		int score = fu.first.get();
		threadcount--;
		if (max_score < score)
		{
				max_score = score;
				max_pos = fu.second;
		
		}
	}
	if (level == 0)
	{
		std::cout << "max score : " << max_score << std::endl;
		std::cout << "position : " << max_pos << std::endl;
		max_score = max_pos;

	}
	//std::cout << "<-- back from level : " << level << std::endl;
	return max_score;
}



void play()
{

	

	
	if (!out_moves.is_open())
		return;

	int short computer_play_side = 0;  // 0 lower , 1 upper
	board b;
	b.reset();
	b.prepare_cache();
	b.validate_cache();

	threadcount.store(0);

	std::vector<board> moves;
	moves.push_back(b);

	while (!b.end())
	{
		//player
		b.print();
		std::cout << "player 1" << std::endl;
		int pot;
		std::cin >> pot;

		if (pot < 0)
		{
			for (int i = 0; i <= -pot; i++)
			{
				b = moves.back();
				moves.pop_back();

			}

		}
		else
		{
			b.move(pot, b);
			moves.push_back(b);

		}

		if (b.end())
			break;

		/*b.print();
		std::cout << "player 2" << std::endl;
		std::cin >> pot;
		b.move(pot, b);*/


		//comp
		b.print();
		std::cout << "player 2" << std::endl;
		op_count.store(0);
		
		moves_map.clear();
		auto start = std::chrono::high_resolution_clock::now();
		b.move(think(b, 0, computer_play_side), b);
		

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		double diff_cout = diff.count();
		auto num_of_operation = op_count.load();
		double op_per_sec = num_of_operation / diff_cout;
		std::cout << "time = " << diff_cout<< " num of ops = " << num_of_operation << "   pope pre sec = " << op_per_sec <<  std::endl;
		//=============================
		std::cout << "------------" << std::endl;
		out_moves << "------------" << std::endl;
		/*for (auto const& a : moves_map)
		{
			std::cout << "\nlevel - " << a.first << " : ";
			out_moves << "\nlevel - " << a.first << " : ";
			for (auto const& b : a.second)
			{
				std::cout << b << " , ";
				out_moves << b << " , ";
			}
		}*/
		/*std::sort(scores_map.begin(), scores_map.end());
		std::cout << "\nscores - ";
		for (auto const& a : scores_map)
		{
			std::cout << a << ',';
		}
		std::cout << std::endl;*/
		//=====================================

		out_moves.close();

		moves.push_back(b);

	}
	std::cout << "*** end game***" << std::endl;

	
	

	b.print();

}



int main()
{

	threadcount.store(0);
	play();
	
	
	

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
