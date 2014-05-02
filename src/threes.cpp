/**
 * Threes! player
 * Author: 21006694 Monty Galloway 2014
 */
#include <threes.h>

std::vector<int> input_sequence;
int tile_num = 0;

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

/**
 * Read in the initial board values and input sequence for game of Threes!
 * @param board          Board container to initialize
 * @param input_sequence Game input sequence of tiles
 * @param file_name      File name for the input file
 */
void read_in_file(std::vector< std::vector<int> > *board, std::vector<int> *input_sequence, char *file_name) {
  std::ifstream in_file;
  in_file.open(file_name);
  if (in_file.is_open()) {
    std::string line;
    // skip first 2 lines for now
    std::getline(in_file, line);
    std::getline(in_file, line);

    // read board init values
    for (int row = 0; row < BOARD_SIZE; row++) {
      std::getline(in_file, line);
      std::istringstream iss(line);
      for (int col = 0; col < BOARD_SIZE; col++) {
        int val;
        iss >> val;
        (*board)[row][col] = val;
      }
    }

    // skip blank line 7
    std::getline(in_file, line);
    // read input sequence
    while (!in_file.eof()) {
      std::getline(in_file, line);
      std::istringstream iss(line);
      std::string string_val;
      while (std::getline(iss, string_val, ' ')) {
        if (!std::isdigit(string_val[0])) continue; // check for white space
        input_sequence->push_back(std::stoi(string_val));
      }
    }

  } else {
    std::cout << "Could not open file: \"" << file_name << "\"\n";
    std::exit(EXIT_FAILURE);
  }
}

/**
 * Print the board to stdout
 * @param board Board container to be printed
 */
void print_board(const std::vector< std::vector<int> > &board) {
  for (std::vector<int> l : board) {
    for (int c : l)
      std::cout << std::setfill(' ') << std::setw(4) << c << " ";
    std::cout << "\n";
  }
  std::cout << "\n";
}

/**
 * Check if two tiles can move or combine
 * @param  lv last tile value (i.e adjacent)
 * @param  v  current tile value
 * @return 1 if shift performed, 0 otherwise
 */
int eval_tiles(int *lv, int *v) {
  if (*lv == EMPTY) {
    *lv = *v;
    *v = EMPTY;
  } else if (*lv == *v && *v > 2) {
    *lv *= 2;
    *v = EMPTY;
  } else if (*lv + *v == 3) {
    *lv = 3;
    *v = EMPTY;
  } else {
    return 0;
  }
  return 1;
}

void get_shift_string(std::vector< std::vector<int> > &board, Shift &s) {
  std::string lexi_string;
  if (s.m == U) {
    for (int row = BOARD_SIZE - 1; row >= 0; row--) {
      lexi_string += std::to_string(board[row][s.id]);
    }
  } else if (s.m == D) {
    for (int row = 0; row < BOARD_SIZE; row++) {
      lexi_string += std::to_string(board[row][s.id]);
    }
  } else if (s.m == L) {
    for (int col = BOARD_SIZE - 1; col >= 0; col--) {
      lexi_string += std::to_string(board[s.id][col]);
    }
  } else {
    for (int col = 0; col < BOARD_SIZE; col++) {
      lexi_string += std::to_string(board[s.id][col]);
    }
  }
  s.shift_string = lexi_string;
}

int tile_score(int val) {
  if (val == 0) return 0;
  if (val == 1 || val == 2) return 1;
  return std::pow(3, (int)std::log2((double) val / 3.0) + 1);
}

int score(const std::vector< std::vector<int> > &board) {
  int score = 0;
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      score += tile_score(board[row][col]);
    }
  }
  return score;
}

std::vector<Direction> get_possible_moves(const std::vector< std::vector<int> > &board, int tile) {
  std::vector<Direction> poss_moves;

  std::vector<Direction> move_space;
  move_space.push_back(U);
  move_space.push_back(D);
  move_space.push_back(L);
  move_space.push_back(R);

  for (Direction d : move_space) {
    std::vector< std::vector<int> > b_copy = board;
    std::vector<Shift> shifts = make_move(&b_copy, d, tile);
    if (shifts.size() > 0) poss_moves.push_back(d);
  }

  return poss_moves;
}

int greedy_search(std::vector< std::vector<int> > &board, int depth, int tile) {
  if (depth == 0) return score(board);
  std::vector<Direction> poss_moves = get_possible_moves(board, tile);
  if (poss_moves.size() == 0) return score(board);
  int best_val = -1;
  for (Direction m : poss_moves) {
    std::vector< std::vector<int> > b = board;
    make_move(&b, m, tile); // possibly add shifts # to eval total
    best_val = std::max(best_val, greedy_search(b, depth - 1, (tile + 1) % (input_sequence.size() - 1)));
  }
  return best_val;
}

/**
 * [make_move description]
 * @param  board [description]
 * @param  move  [description]
 * @return       [description]
 */
std::vector<Shift> make_move(std::vector< std::vector<int> > *board, Direction move, int tile) {
  std::vector<Shift> shifts;
  switch(move) {
    case U:
      for (int col = 0; col < BOARD_SIZE; col++) {
        int num_shifts = 0;
        for (int row = 1; row < BOARD_SIZE; row++) {
          int *lv = &(*board)[row - 1][col];
          int *v = &(*board)[row][col];
          if (*v == EMPTY) continue;
          num_shifts += eval_tiles(lv, v);
        }
        if (num_shifts > 0) {
          Shift s(U, col);
          get_shift_string(*board, s);
          shifts.push_back(s);
        }
      }
      break;
    case D:
      for (int col = 0; col < BOARD_SIZE; col++) {
        int num_shifts = 0;
        for (int row = BOARD_SIZE - 2; row >= 0; row--) {
          int *lv = &(*board)[row + 1][col];
          int *v = &(*board)[row][col];
          if (*v == EMPTY) continue;
          num_shifts += eval_tiles(lv, v);
        }
        if (num_shifts > 0) {
          Shift s(D, col);
          get_shift_string(*board, s);
          shifts.push_back(s);
        }
      }
      break;
    case L:
      for (int row = 0; row < BOARD_SIZE; row++) {
        int num_shifts = 0;
        for (int col = 1; col < BOARD_SIZE; col++) {
          int *lv = &(*board)[row][col - 1];
          int *v = &(*board)[row][col];
          if (*v == EMPTY) continue;
          num_shifts += eval_tiles(lv, v);
        }
        if (num_shifts > 0) {
          Shift s(L, row);
          get_shift_string(*board, s);
          shifts.push_back(s);
        }
      }
      break;
    case R:
      for (int row = 0; row < BOARD_SIZE; row++) {
        int num_shifts = 0;
        for (int col = BOARD_SIZE - 2; col >= 0; col--) {
          int *lv = &(*board)[row][col + 1];
          int *v = &(*board)[row][col];
          if (*v == EMPTY) continue;
          num_shifts += eval_tiles(lv, v);
        }
        if (num_shifts > 0) {
          Shift s(R, row);
          get_shift_string(*board, s);
          shifts.push_back(s);
        }
      }
      break;
    default:
      break;
  }
  // std::cout << "|| adding tile: " << input_sequence[tile_num] << "\n";
  if (shifts.size() != 0) 
    // std::cout << "No shift occurred, couldn't add new tile\n";
    // return shifts;
  // }
    add_tile(board, shifts, input_sequence[tile]);
  return shifts;
}

void add_tile(std::vector< std::vector<int> > *board, 
  std::vector<Shift> &shifts, int tile) {
  std::sort(shifts.begin(), shifts.end());

  Direction shift_d = shifts[0].m;
  int shift_id = shifts[0].id;

  if (shift_d == U) {
    (*board)[BOARD_SIZE - 1][shift_id] = tile;
  } else if (shift_d == D) {
    (*board)[0][shift_id] = tile;
  } else if (shift_d == L) {
    (*board)[shift_id][BOARD_SIZE - 1] = tile;
  } else {
    (*board)[shift_id][0] = tile;
  }
}

/**
 * Play Threes!
 */
int main(int argc, char *argv[]) {

  std::vector< std::vector<int> > board (BOARD_SIZE, 
                                          std::vector<int>(BOARD_SIZE, EMPTY));
  std::vector<std::string> move_sequence;

  bool manual_play = false;
  bool random = false;
  bool greedy = true;

  std::map<std::string, Direction> move_parse;
  move_parse.insert(std::pair<std::string, Direction>("U", U));
  move_parse.insert(std::pair<std::string, Direction>("D", D));
  move_parse.insert(std::pair<std::string, Direction>("L", L));
  move_parse.insert(std::pair<std::string, Direction>("R", R));

  std::map<Direction, std::string> parse_move;
  parse_move.insert(std::pair<Direction, std::string>(U, "U"));
  parse_move.insert(std::pair<Direction, std::string>(D, "D"));
  parse_move.insert(std::pair<Direction, std::string>(L, "L"));
  parse_move.insert(std::pair<Direction, std::string>(R, "R"));

  // static const int input_space[] = {1, 2, 3, 6, 12, 24, 48};

  if (argc < 2) {
    std::cout << "Usage: threes in_file_name\n";
    std::exit(EXIT_FAILURE);
  }

  char *file_name = argv[1];

  read_in_file(&board, &input_sequence, file_name);

  print_board(board);
  std::string move;

  std::srand(std::time(NULL));
  // int next_tile;
  // next_tile = input_sequence.front(); input_sequence.pop();

  std::vector<Direction> poss_moves = get_possible_moves(board, tile_num);
  while (poss_moves.size() > 0) {
    std::cout << tile_num << " " << input_sequence[tile_num] << " " << input_sequence.size() << "\n";
    print_board(board);
    std::cout << "Possible moves: ";
    for (Direction d : poss_moves)
      std::cout << parse_move.find(d)->second << " ";
    std::cout << "\n";
    Direction m;
    if (manual_play) {
      std::cin >> move;
      m = move_parse.find(move)->second;
    } else if (random) {
      int rand_move = (int)(std::rand() % poss_moves.size());
      std::cout << "Chose poss[" << rand_move << "]: ";
      std::cout << parse_move.find(m)->second << "\n";
      m = poss_moves[rand_move];
    } else {
      std::priority_queue<std::pair<int, Direction>, std::vector<std::pair<int, Direction>>, comparator> pq; 
      for (Direction d : poss_moves) {
        std::vector< std::vector<int> > b = board;
        make_move(&b, d, tile_num);
        pq.push(std::pair<int, Direction>(greedy_search(b, 2, tile_num ), d));
      }
      std::cout << pq.top().first << "\n";
      m = pq.top().second;
      while (!pq.empty()) {
        std::cout << "(" << pq.top().first << ", " << parse_move.find(pq.top().second)->second << ") ";
        pq.pop();
      }
      std::cout << "\n";
    }

    make_move(&board, m, tile_num);
    // tile_num = (tile_num + 1) % (input_sequence.size() - 1);
    tile_num++;
    if (tile_num > input_sequence.size() - 1) {
      std::cout << "Ran out of tiles.\n";
      break;
    }

    move_sequence.push_back(parse_move.find(m)->second);

    poss_moves = get_possible_moves(board, tile_num);
  }
  
  print_board(board);
  std::cout << "Game over. Score is: " << score(board) << ". Moves were: \n";
  for (std::string m : move_sequence) 
    std::cout << m;
  std::cout << "\n";

  return 0;
}
