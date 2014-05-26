#ifndef threes_AI_threes_AI_h
#define threes_AI_threes_AI_h
#include <threes_IO.h>
#include <threes_Mechanics.h>

/*
 *******************************************************************************
 * function declaration *
 *******************************************************************************
*/

/* dfs on board for move path */
std::vector<Node> dfs(Board &);
int greedy_search(Board &, int, int);

#endif