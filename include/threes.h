/**
 * 
 */
#ifndef threes_threes_h
#define threes_threes_h
/** Includes **/ 
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <iomanip>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <string> 
#include <iostream>
#include <sstream>
#include <fstream> 
#include <cstdlib>
#include <cmath>
#include <ctime>

/** Defines **/ 
#define EMPTY 0
#define BOARD_SIZE 4

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

// struct Move {
//   Direction d;
//   int shifts;
//   bool operator<(const Move &m) const {
//     if (m.shifts > shifts) return true;
//     if (score())
//   }
// };



/** function declaration **/ 
void add_tile(std::vector< std::vector<int> > *, std::vector<Shift> &, int);
std::vector<Shift> make_move(std::vector< std::vector<int> > *, Direction, int);
int eval_tiles(int *, int *);
void print_board(const std::vector< std::vector<int> > &);
void read_in_file(std::vector< std::vector<int> > *, std::queue<int> *, char *);
std::vector<Direction> get_possible_moves(const std::vector< std::vector<int> > &, int);
int score(const Board &);
#endif