#include <threes_AI.h>

/* Setup move parser maps, mapping Direction to string and vice versa */
void initMoveParsers(std::map<std::string, Direction> &move_parse,
                    std::map<Direction, std::string> &parse_move) {

  move_parse.insert(std::pair<std::string, Direction>("U", U));
  move_parse.insert(std::pair<std::string, Direction>("D", D));
  move_parse.insert(std::pair<std::string, Direction>("L", L));
  move_parse.insert(std::pair<std::string, Direction>("R", R));

  parse_move.insert(std::pair<Direction, std::string>(U, "U"));
  parse_move.insert(std::pair<Direction, std::string>(D, "D"));
  parse_move.insert(std::pair<Direction, std::string>(L, "L"));
  parse_move.insert(std::pair<Direction, std::string>(R, "R"));
}

std::string dToStr(Direction d) {
  switch (d) {
    case U:
      return "U";
    case D:
      return "D";
    case L:
      return "L";
    case R:
      return "R";
    default:
      std::cout << "Error in dToStr\n";
      exit(EXIT_FAILURE);
  }
}

Direction strToD(std::string str) {
  std::map<std::string, Direction> move_parse;
  move_parse.insert(std::pair<std::string, Direction>("U", U));
  move_parse.insert(std::pair<std::string, Direction>("D", D));
  move_parse.insert(std::pair<std::string, Direction>("L", L));
  move_parse.insert(std::pair<std::string, Direction>("R", R));

  return move_parse.find(str)->second;
}
/* Greedy best first search hill climb. Selects next move based on highest
 * board score
//  */
// void hillClimb(Board &board) {
//   PQ moveQueue;
//   // std::vector<Direction> parents = std::vector<Direction>(inputSequence.size());
//   std::vector<Direction> possMoves = getPossibleMoves(board, tile_num);

//   for (Direction d : possMoves) {
//     moveQueue.push()
//   }

//   while (!moveQueue.empty()) {

//   }
// }

/* calculates number of tiles that merged in move from board1 -> board2, returns
 * 0 if no tiles were consumed in the move (i.e. an input tile was added)
 */
int numberCollapsed(Board &b1, Board &b2) {

  int numNonZeroB1 = 0;
  int numNonZeroB2 = 0;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      numNonZeroB1 += b1[row][col] > 0 ? 1 : 0;
      numNonZeroB2 += b2[row][col] > 0 ? 1 : 0;
    }
  }

  return std::max(numNonZeroB1 - numNonZeroB2, 0);
}

int nonZeroTiles(Board &b) {
  int numNonZero = 0;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      numNonZero += b[row][col] > 0 ? 1 : 0;
    }
  }
  return numNonZero;
}

std::string boardToString(Board &b) {
  std::string str = "";

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      str += std::to_string(b[row][col]);
    }
  }
  return str;
}

int THEORETICAL_HIGHSCORE;
int a_star(Board board, std::vector<std::string> &move_sequence, int depth, int *moves) {
  // goal state is tile_num = inputSequence.size() - 1
  // f(n) = 1 (cost of adding tile) + number of tiles collapsed?
  int maxDepth = 0;
  std::map<std::string, Direction> move_parse;
  std::map<Direction, std::string> parse_move;
  initMoveParsers(move_parse, parse_move);

  std::map<std::string, Node> parentMap;

  Node root;
  root.parent = NULL;
  root.b = board;
  root.depth = tile_num;
  root.f = 0;
  root.id = 0;
  root.score = score(root.b);
  root.str = boardToString(root.b);
  // getPossibleMoves(root.b, root.depth);

  parentMap.insert(std::pair<std::string, Node>(root.str, root));
  std::vector<Node> parents = std::vector<Node>(depth);
  NodeQ nq;
  nq.push(root);
  Node *maxNode;
  int maxScore = -1;
  while (!nq.empty()) {
    Node top = nq.top(); nq.pop();
    // std::cout << top.score << "\n"; 
    // std::cout << top.f << "\n";
    // if (top.depth == inputSequence.size() - 1) { // depth limit to # input tiles
    if (top.depth - root.depth == depth) { // depth limit to # input tiles
      if (top.score > maxScore) {
        // *moves = top.depth;
        maxScore = top.score;
        // move_sequence[top.depth] = parse_move.find(top.moveMade)->second;
        // maxDepth = top.depth;
        maxNode = &top;
        // std::cout << "Yo\n";
      }
      continue;
    }

    maxDepth = std::max(maxDepth, top.depth - tile_num);
    // store move sequence unless we have root node, which will 
    // have no previous move
    // if (top.id != root.id) { 
      // move_sequence[top.depth] = parse_move.find(top.moveMade)->second;
    // }

    // get frontier of current node
    std::vector<Direction> possMoves = getPossibleMoves(top.b, top.depth);

    if (possMoves.size() == 0) {
      // if (parentMap.size() > 0)
        // parentMap.erase(parentMap.find(top.str));
      continue;
    }
    // std::cout << top.depth << "\n";
    int id = 0;
    for (Direction d: possMoves) { // push moves as nodes to queue
      Node n;
      n.b = top.b;
      n.id = top.id + 1 + id++;
      n.depth = top.depth + 1;
      makeMove(&n.b, d, n.depth); // make move
      n.parent = &top;
      n.moveMade = d;
      n.score = score(n.b);
      n.g = top.g + 1; 
      // n.h = std::pow(numberCollapsed(top.b, n.b), 2); // this is where the heuristic matters
      // n.h = std::pow(nonZeroTiles(n.b), 2);
      // n.h = 0; // uniform cost search
      // n.h = n.score;
      // n.h  = nonZeroTiles(n.b);
      // n.h = score(n.b);
      // std::cout << n.h << "\n";
      // n.f = std::max(n.h + n.g, top.f);
      // n.f = std::min(n.h + n.g, top.f);
      n.f = score(n.b); // greedy best first search
      n.str = boardToString(n.b);
      // std::cout << n.depth - root.depth -1 << "\n";
      parents[n.depth - root.depth - 1] = top;

      // if (parentMap.count(n.str) == 0) 
      //   parentMap.insert(std::pair<std::string, Node>(n.str, top));
      // else {
      //   if (parentMap.find(n.str)->second.f > n.f) {
      //     parentMap.erase(parentMap.find(n.str));
      //     parentMap.insert(std::pair<std::string, Node>(n.str, top));
      //   }
      // }
      nq.push(n);
    }
    // std::cout << parentMap.size() << "\n";
  }
  *moves = maxDepth;
    // std::cout << "max: " << maxDepth << "\n";
  // make the actual moves on the initial board
  // for (int i = 1; i < *moves; i++) {
  //   std::cout << move_sequence[i];
  //   makeMove(&board, move_parse.find(move_sequence[i])->second, i);
  //   // printBoard(board);
  // }
  // Node p = *maxNode;
  int mm = maxDepth;
  // std::cout << "mm " << mm << "\n";
  while (mm > 0) {
    Node p = parents[mm--];
    move_sequence.push_back(parse_move.find(p.moveMade)->second);
  }
    
  // while (p.str != root.str) {
  //   // std::cout << parse_move.find(p.moveMade)->second;
  //   p = parentMap.find(p.str)->second;
  // }
  // parentMap.clear();
  std::reverse(move_sequence.begin(), move_sequence.end());
  std::cout <<"move_sequence.size(): " << move_sequence.size() << "\n";
  // std::vector<Direction> movess;
  // while (p->parent != NULL) {
  //   // printBoard(p.b);
  //   std::cout << parse_move.find(p->moveMade)->second;
  //   // makeMove(&board, p.moveMade)
  //   p = p->parent;
  // }

  std::cout << "\n";

  return !(tile_num < inputSequence.size() - 1);
}

int i_aStar(Board &board, std::vector<std::string> &move_sequence) {
  THEORETICAL_HIGHSCORE = inputSequence.size()*inputSequence.size();
  // THEORETICAL_HIGHSCORE = 1;

  std::cout << "Goal: " <<  THEORETICAL_HIGHSCORE << "\n";
  std::map<std::string, Direction> move_parse;
  std::map<Direction, std::string> parse_move;
  initMoveParsers(move_parse, parse_move);
  int numMoves = 0;
  int DEPTH_LIMIT = 8;

  // a_star(board, move_sequence, DEPTH_LIMIT, &numMoves);
  // int tt = 0;
  // for (std::string s: move_sequence) {
  //   makeMove(&board, move_parse.find(s)->second, tt++);
  // }
  // return 0;
  // std::vector<std::string> allMoves;
  while (numMoves < inputSequence.size()) {
    std::vector<std::string> ms;
    // std::cout << score(board) << "\n";
    int moves = 0;
    a_star(board, ms, DEPTH_LIMIT, &moves);
    numMoves += ms.size();
    if (ms.size() == 0) break;
    for (int i = 0; i < ms.size(); i++) {
      move_sequence.push_back(ms[i]);
      makeMove(&board, move_parse.find(ms[i])->second, tile_num++);
      printBoard(board);
    }
    // tile_num += moves;
  }
  std::cout << "inputSequence.size(): " << inputSequence.size() << "\n";
  std::cout << "moves: " << numMoves << "\n";
  return !(numMoves < inputSequence.size());
}

/*
 *
 * Design Possibilities:
 *    Select moves based on minimizing number of tiles on the board (consequently
 *     maximizing tile combinations)
 *
 *    Select next move based on maximizing board score (greedy best first search)
 * 
 */

// 
//  heuristic function h(b)
//  -> greedy local search, choose greatest h(b) until all h(b+1) < h(b), then
//      go back to h(b - 1) and choose next best h(b)
// h(b) = #shifts? board eval?
// 
// Node {
//  priorty_queue poss_moves
//  Node *parent
// }
int dfs(Board &board, std::vector<std::string> move_sequence, int depthLimit) {
    // Use these maps to convert strings to Direction enums, and vice versa.


  move_sequence = std::vector<std::string>(inputSequence.size());

  std::vector<Node> parents = std::vector<Node>(inputSequence.size());
  Node root; 
  root.b = board;
  // root.poss_moves = PQ();
  root.parent = NULL;
  root.depth = 0;

  // parents[0] = root;
  int max_score = -1;
  std::stack<Node> s;
  s.push(root);
  while (!s.empty()) {
    Node top = s.top(); s.pop();
    parents[top.depth] = top;
    if (top.depth == inputSequence.size() - 1) {
      // std::cout << "Score: " << score(top.b) << "\n";
      max_score = std::max(score(top.b), max_score);
      continue;
    }
    if (top.depth == depthLimit) continue;
    PQ poss_moves = getPossibleMovesSorted(top.b,  top.depth + 1);

    if (poss_moves.size() == 0) continue;

    for (int i = 0; i < poss_moves.size(); i++) {
      Direction d = poss_moves.top().second; poss_moves.pop();
      Node n;
      n.b = top.b;
      n.depth = top.depth + 1;
      makeMove(&n.b, d, top.depth + 1);
      s.push(n);
    }

  }
  return 0;
}

// int depthLimitedDFSSearch(Board board, int depth) {
//   std::stack<Node> path;
//   std::stack<Node> frontier;

//   Node root;
//   root.b = board;
//   root.
// }

// int greedy_search(Board &board, int depth, int tile) {
//   if (depth == 0) return score(board);
//   std::vector<Direction> poss_moves = getPossibleMoves(board, tile);
//   if (poss_moves.size() == 0) return score(board);
//   int best_val = -1;
//   for (Direction m : poss_moves) {
//     Board b = board;
//     makeMove(&b, m, tile); // possibly add shifts # to eval total
//     best_val = std::max(best_val, greedy_search(b, depth - 1, (tile + 1) % (inputSequence.size() - 1)));
//   }
//   return best_val;
// }
// 
// 

Direction greedy_search2(Board board, int tile) {
  // std::vector<Direction> poss_moves = getPossibleMoves(board, tile);

  int depthLim = 3;

  std::stack<Node> ns;

  Node root;
  // root.parent = NULL;
  root.b = board;
  // root.moveMade = NULL;
  root.depth = 0;
  root.score = score(board);
  root.isRoot = true;
  ns.push(root);

  Node maxNode = root;

  while (!ns.empty()) {
    Node top = ns.top(); ns.pop();

    std::vector<Direction> possMoves = getPossibleMoves(top.b, tile);

    if (top.depth == depthLim) {
      if (maxNode->score < top.score) {
        maxNode = top;
        printBoard(maxNode->b);
      }
      continue;
    }
    for (Direction d : possMoves) {
      Node n;
      n.b = top.b;
      makeMove(&n.b, d, tile);

      n.moveMade = d;
      n.depth = top.depth + 1;
      n.score = score(n.b);
      n.parent = &top;
      n.isRoot = false;
      ns.push(n);
    }

    tile++;
  }
  // std::cout << "Printing:\n";
  // printBoard(maxNode->b);
  // std::cout << "Printing:\n";
  // printBoard(maxNode->parent->b);
  // std::cout << "Root: \n";
  // printBoard(root.b);
  // exit(0);

  Node *p = maxNode;

  std::stack<Direction> moveStack;

  // while (!p->isRoot) {
  for (int i = 0; i < depthLim; i++) {
    moveStack.push(p->moveMade);
    std::cout << dToStr(p->moveMade) << "\n";
    p = p->parent;
  }

  Direction m;
  while (moveStack.size() > 1) {
    m = moveStack.top();
    std::cout << dToStr(moveStack.top());
    moveStack.pop();
  }
  std::cout << "\n";
  return m;
}

Direction greedy_search(Board board, int tile){
  std::vector<Direction> poss_moves = getPossibleMoves(board, tile);
  int sss = 0;
  int tile1 = tile + 1;
  int tile2 = tile1 + 1;
  int tile3 = tile2 + 1;
  int tile4 = tile3 + 1;
  int tile5 = tile4 + 1;
  int tile6 = tile5 + 1;
  int tile7 = tile6 + 1;
  Direction ddd;
  for (Direction m : poss_moves) {
    std::vector< std::vector<int> > b_copy = board;
    makeMove(&b_copy, m, tile);
    std::vector<Direction> poss_moves1 = getPossibleMoves(b_copy, tile1);
    if(poss_moves1.size()==0){
        if(score(b_copy)>sss){
          sss = score(b_copy);
          ddd = m;
          printf("Direction : %i \n", ddd);
        }
        continue;
    }
    for(Direction n : poss_moves1) {
      std::vector< std::vector<int> > c_copy = b_copy;
      makeMove(&c_copy, n, tile1);
      std::vector<Direction> poss_moves2 = getPossibleMoves(c_copy, tile2);
      if(poss_moves2.size()==0){
        if(score(c_copy)>sss){
          sss = score(c_copy);
          ddd = m;
          printf("Direction : %i \n", ddd);
        }
        continue;
      }
      for(Direction l : poss_moves2) {
        std::vector< std::vector<int> > d_copy = c_copy;
        makeMove(&d_copy, l, tile2);
        std::vector<Direction> poss_moves3 = getPossibleMoves(d_copy, tile3);
        if(poss_moves3.size()==0){
          if(score(d_copy)>sss){
            sss = score(d_copy);
            ddd = m;
            printf("Direction : %i \n", ddd);
          }
          continue;
        } 
        for(Direction k : poss_moves3) {
          std::vector< std::vector<int> > e_copy = d_copy;
          makeMove(&e_copy, k, tile3);
          std::vector<Direction> poss_moves4 = getPossibleMoves(e_copy, tile4);
          if(poss_moves4.size()==0){
            if(score(e_copy)>sss){
              sss = score(e_copy);
              ddd = m;
              printf("Direction : %i \n", ddd);
            }
            continue;
          } 
          for(Direction j : poss_moves4) {
            std::vector< std::vector<int> > f_copy = e_copy;
            makeMove(&f_copy, j, tile4);
            std::vector<Direction> poss_moves5 = getPossibleMoves(f_copy, tile5);
            if(poss_moves5.size()==0){
              if(score(f_copy)>sss){
                sss = score(f_copy);
                ddd = m;
                printf("Direction : %i \n", ddd);
              }
              continue;
            } 
            for(Direction o : poss_moves5) {
              std::vector< std::vector<int> > g_copy = f_copy;
              makeMove(&g_copy, o, tile5);
              std::vector<Direction> poss_moves6 = getPossibleMoves(g_copy, tile6);
              if(poss_moves6.size()==0){
                if(score(g_copy)>sss){
                  sss = score(g_copy);
                  ddd = m;
                  printf("Direction : %i \n", ddd);
                }
                continue;
              } 
              for(Direction p : poss_moves6) {
                std::vector< std::vector<int> > h_copy = g_copy;
                makeMove(&h_copy, p, tile6);
                std::vector<Direction> poss_moves7 = getPossibleMoves(h_copy, tile7);
                if(poss_moves7.size()==0){
                  if(score(h_copy)>sss){
                    sss = score(h_copy);
                    ddd = m;
                    printf("Direction : %i \n", ddd);
                  }
                  continue;
                } 
                for(Direction q : poss_moves7) {
                  std::vector< std::vector<int> > i_copy = h_copy;
                  makeMove(&i_copy, q, tile7);
                  if(score(i_copy)>sss){
                    sss = score(i_copy);
                    ddd = m;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return ddd;
}
