#ifndef threes_AI_threes_AI_h
#define threes_AI_threes_AI_h
#include <threes_IO.h>
#include <threes_Mechanics.h>
#include <chrono>
#include <thread>

/*
 *******************************************************************************
 * function declaration *
 *******************************************************************************
*/

/* dfs on board for move path */
std::vector<Node> dfs(Board &);
//int greedy_search(Board, int, int);
Direction greedy_search(Board,int);

#endif