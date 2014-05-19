/**
 * 
 */
#ifndef threes_Mechanics_threes_Mechanics_h
#define threes_Mechanics_threes_Mechanics_h
/** Includes **/ 
#include <algorithm>

#include <vector>
#include <queue>
#include <set>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>

#include <string> 

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream> 
 
#include <cstdlib>
#include <cmath>
#include <ctime>

/** Defines **/ 
#define EMPTY 0
#define BOARD_SIZE 4

extern std::vector<int> inputSequence;
extern int tile_num;

/** 
 * move direction representation
 */
enum Direction {
  U, D, L, R
};

class comparator {
  bool reverse;
public:
  comparator(const bool & revparam=false){
    reverse=revparam;
  }
  bool operator() (const std::pair<int,Direction>& m1, const std::pair<int, Direction>& m2) {
    if (reverse) return m1.first > m2.first;
    return m1.first < m2.first;
  }
};

/** Board data structure **/ 
typedef std::vector< std::vector<int> > Board;

typedef std::priority_queue<std::pair<int, Direction>, std::vector<std::pair<int, Direction>>, comparator> PQ;

/** Node for DFS tree **/ 
struct Node {
  PQ poss_moves;
  Node *parent;
  Board b;
  int depth;
};

/**
 * data structure to represent shift.
 */
struct Shift {
  int id; // describes the row/col that was shifted (id in [0-3])
  Direction m; // determines if id is row or col (D/U: col, L/R: row)
  // string representation of shift - allows lexicographic sorting of shifts
  std::string shift_string; 

  // constructor
  Shift(Direction mm, int idid) : id(idid), m(mm) {}

  // pretty much lexicographical, with precedence given to most clockwise
  // position relative to move direction (in the case of matching
  // lexicographical score)
  bool operator<(const Shift &a)  const {
    // if (a.shift_string.length() > shift_string.length()) return true;

    if (a.shift_string.compare(shift_string) > 0) return true;
    
    if (a.shift_string.compare(shift_string) == 0) {
      // right most or lowest for Up or Left move, respectively
      if (m == U || m == L) return a.id > id;
      // left most or highest for Down or Right move, respectively
      else return a.id < id;
    }

    return false;

    // return a.shift_string.compare(shift_string) > 0 ? true : a.shift_string.compare(shift_string) == 0 ? a.id < id : false;
  }

};


/*
 *******************************************************************************
 * function declaration *
 *******************************************************************************
*/

/* add tile to board */
void addTile(std::vector< std::vector<int> > *, std::vector<Shift> &, int);

/* 
 * check if two tiles can make a valid move (i.e add doubles, add 1 and 2
 * or simply move to empty space) and make the change.
 *
 * Returns 1 if move is made, 0 otherwise
 */
int tileMove(int *, int *);

/* Set the shift string of Shift's columns/rows, so that we can sort them */
void setShiftString(Board &, Shift &);

/* Get legal moves on board given input tile */
std::vector<Direction> getPossibleMoves(const Board &, int);
PQ getPossibleMovesSorted(const Board &, int);

/* Make move specified by Direction on Board with input tile. Returns shifts */
std::vector<Shift> makeMove(Board *, Direction, int);

void print_board(const std::vector< std::vector<int> > &);
void read_in_file(std::vector< std::vector<int> > *, std::queue<int> *, char *);

/* Calculate score of tile */ 
int tileScore(int val);

/* Calculate score of board, based on sum of tileScore(tileVal) */
int score(const Board &);
#endif