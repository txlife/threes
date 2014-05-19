/**
 * Threes! player
 * Author: 21006694 Monty Galloway 2014
 */
#include <threes.h>

/**
 * Play Threes!
 * usage: 
 *   threes /dir/to/input_file_name [1|2]
 *     1: Manual moves (input U, D, L, or R for each turn)
 *     2: Random move selection
 */
int main(int argc, char *argv[]) {
  // std::vector<int> inputSequence;
  // int tile_num = 0;
  Board board (BOARD_SIZE, std::vector<int>(BOARD_SIZE, EMPTY));
  std::vector<std::string> move_sequence;
  
  if (argc < 2) {
    std::cout << "Usage: threes /dir/to/input_file_name [1 | 2]\n";
    std::cout << "\t1: Manual moves (input U, D, L, or R for each turn)\n";
    std::cout << "\t2: Random move selection\n";
    std::exit(EXIT_FAILURE);
  }

  int play_type = 0;

  if (argc > 2) {
    play_type = std::stoi(argv[2]);

    if (!(play_type == 1 || play_type == 2)) {
      std::cout << "Play type should be 1 or 2, omit for AI algorithm\n";
      std::exit(EXIT_FAILURE);
    }
  }

  bool manual_play = play_type == 1;
  bool random = play_type == 2;

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



  char *file_name = argv[1];

  readInFile(&board, &inputSequence, file_name);

  std::cout << "Branching factor of 4, with depth: " << inputSequence.size();
  std::cout << " gives us: O(" << std::pow(4, inputSequence.size()) << ")\n";
  printBoard(board);
  std::string move;

  std::srand(std::time(NULL));
  // int next_tile;
  // next_tile = inputSequence.front(); inputSequence.pop();

  std::vector<Direction> poss_moves = getPossibleMoves(board, tile_num);
  while (poss_moves.size() > 0) {
    std::cout << tile_num << " " << inputSequence[tile_num] << " " << inputSequence.size() << "\n";
    printBoard(board);
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
      // PQ pq; 
      // for (Direction d : poss_moves) {
      //   std::vector< std::vector<int> > b = board;
      //   makeMove(&b, d, tile_num);
      //   // pq.push(std::pair<int, Direction>(greedy_search(b, 2, tile_num ), d));
      // }
      // std::cout << pq.top().first << "\n";
      // m = pq.top().second;
      // while (!pq.empty()) {
      //   std::cout << "(" << pq.top().first << ", " << parse_move.find(pq.top().second)->second << ") ";
      //   pq.pop();
      // }
      // std::cout << "\n";
      std::vector<Node> path = dfs(board);
      for (Node n : path) {
        printBoard(n.b);
        board = n.b;
      }
      break;
    }

    makeMove(&board, m, tile_num);
    // tile_num = (tile_num + 1) % (inputSequence.size() - 1);
    tile_num++;
    if (tile_num > inputSequence.size() - 1) {
      std::cout << "Ran out of tiles.\n";
      break;
    }

    move_sequence.push_back(parse_move.find(m)->second);

    poss_moves = getPossibleMoves(board, tile_num);
  }
  
  printBoard(board);
  std::cout << "Game over. Score is: " << score(board) << ". Moves were: \n";
  for (std::string m : move_sequence) 
    std::cout << m;
  std::cout << "\n";

  return 0;
}
