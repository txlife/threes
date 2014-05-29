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
 float eval(Node &n) {
  // int s = n.score;
  // int nonZero = nonZeroTiles(n.b);
  int empty = emptyCells(n.b);
  int max = maxonBoard(n.b);
  float smooth = smoothness(n.b);
  int s = std::log(score(n.b))/std::log(3);
  // std::cout << distanceSum - 2.0*std::log(n.score) << "\n";
  // float scoreWeight = 2.0;
  float res = 0.0f;
  res = empty * 0.1 + max * 0.1 + smooth * 0.3 + s * 1.0;
  // float 
  return res;
}
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
Direction greedy_search3(Board board, int tile) {
  // std::vector<Direction> poss_moves = getPossibleMoves(board, tile);

  int depthLim = 5;

  // Node maxNode = root;
  int maxScore = 0;
  std::vector<Direction> originalFrontier = getPossibleMoves(board, tile);
  Direction maxD = originalFrontier[0];
  
  // if (originalFrontier.size() == 1) return maxD; // no point doing dfs

  for (Direction od : originalFrontier) {
    Node root;
    root.b = board;
    makeMove(&root.b, od, tile);
    root.score = 0;
    root.depth = 0;
    root.tileID = tile;
    std::stack<Node> ns;
    ns.push(root);
    while (!ns.empty()) {
      Node top = ns.top(); ns.pop();
      // printf("Tile number : %d    Tile val : %d\n",top.tileID,inputSequence[top.tileID]);
      // printf("Level: %i      top score : %i    maxscore: %i\n", top.depth,top.score,maxScore);
      // //printBoard(top.b);
      // printf("Tile: %d\n",tileID);
      // printf("Tile: %d\n",--tileID);
      if(top.depth > depthLim){
        // exit(EXIT_FAILURE);
      }
      // printf("tile id: %d\n", top.tileID);
      if(top.score < 0){
        printf("Top score < 0\n");
        printf("Level: %i      top score : %i    maxscore: %i   Tile: %d\n", top.depth,top.score,maxScore,top.tileID);

        exit(EXIT_FAILURE);
      }
      if(top.tileID>inputSequence.size()){
        // printf("Level: %i      top score : %i    maxscore: %i   Tile: %d\n", top.depth,top.score,maxScore,tileID);
        printf("TileID--:%d\n",top.tileID);
        continue;
        // exit(EXIT_FAILURE);
      }
      if (top.depth == depthLim) {
        if (maxScore < top.score) {
          maxScore = top.score;
          maxD = od;
          // std::cout << "maxNode board, move is: " << dToStr(maxNode.moveMade) << "\n";
          // printBoard(maxNode.b);
        }
        continue;
      }
      else{
        std::map<int,std::vector<Direction>> mymap;
        mymap[top.depth] = getPossibleMoves(top.b, top.tileID);
        // std::vector<Direction> possMoves = getPossibleMoves(top.b, top.tileID);
        // if(possMoves.size()==0){
        //   if (maxScore < top.score) {
        //     maxScore = top.score;
        //     maxD = od;
        //     // std::cout << "maxNode board, move is: " << dToStr(maxNode.moveMade) << "\n";
        //     // printBoard(maxNode.b);
        //   }
        //   continue;
        // }
        // for (Direction d : possMoves) {
          for (Direction d : mymap[top.depth]) {
          Node n;
          n.b = top.b;
          makeMove(&n.b, d, top.tileID);

          n.moveMade = d;
          n.depth = top.depth + 1;
          n.score = score(n.b);
          n.parent = &top;
          n.tileID = top.tileID + 1;
          if (maxScore < top.score) {
            maxScore = top.score;
            maxD = od;
            // std::cout << "maxNode board, move is: " << dToStr(maxNode.moveMade) << "\n";
            // printBoard(maxNode.b);
          }
          // n.isRoot = false;
          ns.push(n);
        }
      }
    }
  }
  return maxD;
}

Direction greedy_search2(Board board, int tile) {
  // std::vector<Direction> poss_moves = getPossibleMoves(board, tile);

  int depthLim = 6;

  // Node maxNode = root;
  int maxScore = score(board);
  std::vector<Direction> originalFrontier = getPossibleMoves(board, tile);
  Direction maxD = originalFrontier[0];
  
  if (originalFrontier.size() == 1) return maxD; // no point doing dfs

  for (Direction od : originalFrontier) {
    Node root;
    root.b = board;
    makeMove(&root.b, od, tile);
    int tileID = tile;
    root.score = score(board);
    root.depth = 0;
    std::stack<Node> ns;
    ns.push(root);
    while (!ns.empty()) {
      Node top = ns.top(); ns.pop();

      std::vector<Direction> possMoves = getPossibleMoves(top.b, tileID);
      // if(possMoves.size()==0){
      //   if (maxScore < top.score) {
      //     maxScore = top.score;
      //     maxD = od;
      //     // std::cout << "maxNode board, move is: " << dToStr(maxNode.moveMade) << "\n";
      //     // printBoard(maxNode.b);
      //   }
      //   continue;
      // }
      if (top.depth == depthLim) {
        if (maxScore < top.score) {
          maxScore = top.score;
          maxD = od;
          // std::cout << "maxNode board, move is: " << dToStr(maxNode.moveMade) << "\n";
          // printBoard(maxNode.b);
        }
        continue;
      }
      for (Direction d : possMoves) {
        Node n;
        n.b = top.b;
        makeMove(&n.b, d, tileID);

        n.moveMade = d;
        n.depth = top.depth + 1;
        n.score = score(n.b);
        n.parent = &top;
        // n.isRoot = false;
        ns.push(n);
      }

      tileID++;
    }
  }
  return maxD;
  // std::cout << "Printing:\n";
  // printBoard(maxNode->b);
  // std::cout << "Printing:\n";
  // printBoard(maxNode->parent->b);
  // std::cout << "Root: \n";
  // printBoard(root.b);
  // exit(0);

  // Node *p = &maxNode;

  // std::stack<Direction> moveStack;

  // // while (!p->isRoot) {
  // for (int i = 0; i < depthLim; i++) {
  //   moveStack.push(p->moveMade);
  //   std::cout << dToStr(p->moveMade) << "\n";
  //   p = p->parent;
  // }

  // Direction m;
  // while (moveStack.size() > 1) {
  //   m = moveStack.top();
  //   std::cout << dToStr(moveStack.top());
  //   moveStack.pop();
  // }
  // std::cout << "\n";
  // return m;
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
                  // exit(EXIT_FAILURE);
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

/*Ref: https://github.com/ov3y/2048-AI*/
Result minimax(int depth,int alpha,int beta,Board board,int tileID, int cutoffs,bool playerTurn){
  int bestScore = 0;
  Direction bestMove = ERROR;
  Result result;

  //the maxing player
  if(playerTurn){
    bestScore = alpha;
    std::vector<Direction> poss_moves = getPossibleMoves(board, tileID);
    for(Direction p : poss_moves){
      Node n;
      n.b = board;
      makeMove(&n.b, p, tileID);

      if(depth == 0){
        result.move = p;
        result.score = eval(n);
      }
      else{
        // if(tileID+1 > inputSequence.size()){
        //   continue;
        // }
        result = minimax(depth-1, bestScore, beta, n.b, tileID+1, cutoffs, false);
        cutoffs = result.cutoffs;
        tileID = result.tile;
      }

      if(result.score > bestScore){
        bestScore = result.score;
        bestMove = p;
      }
      if(bestScore > beta){
        cutoffs++;
        result.move = bestMove;
        result.score = beta;
        result.tile = tileID;
        result.cutoffs = cutoffs;
        result.b = board;
        return result;
      }
    }
  }

  else{
    bestScore = beta;
    std::vector<Direction> poss_moves = getPossibleMoves(board, tileID);
    for(Direction p : poss_moves){
      Node n;
      n.b = board;
      makeMove(&n.b, p, tileID);

      result = minimax(depth, alpha, bestScore, n.b,tileID+1, cutoffs, true);
      tileID = result.tile;
      cutoffs = result.cutoffs;

      if(result.score < bestScore){
        bestScore = result.score;
      }
      if(bestScore < alpha){
        cutoffs++;
        result.move = ERROR;
        result.score = alpha;
        result.tile = tileID+1;
        result.cutoffs = cutoffs;
        result.b = board;
        return result;
      }
    }
  }
  result.move = bestMove;
  result.score = bestScore;
  result.tile = tileID;
  result.cutoffs = cutoffs;
  result.b = board;
  return result;
}

int maxonBoard(Board board){
  int max = 0;
  for(int x = 0; x < 4; x++){
    for(int y = 0; y < 4; y++){
      if(board[x][y]!=0 && board[x][y]>max){
        max = board[x][y];
      }
    }
  }
  return max;
}

float smoothness(Board board){
  float smooth = 0.0f;
  for(int x = 0; x < 4; x++){
    for(int y = 0; y < 4; y++){
      if(board[x][y]!=0){
        float value = std::log(board[x][y])/std::log(3);
        for(int u = 0; u < 4; u++){
          cell far = findFarthest(board,x,y,u);
          float value1 = std::log(board[far.x][far.y])/std::log(3);
          smooth = smooth - std::abs(value - value1);
        }
      }
    }
  }
  return smooth;
}

int emptyCells(Board board){
  int cell = 0;
  for(int x = 0; x < 4; x++){
    for(int y = 0; y < 4; y++){
      if(board[x][y]==0){
        cell++;
      }
    }
  }
  return cell;
}
cell findFarthest(Board b,int x, int y, int u){
  int max = 0;
  cell far;
  switch (u) {
    case 0:
      while(x >= 1){
        int step = 0;
        x--;
        step++;
        if(b[x][y]!=0 && step > max){
          max = step;
          far.x = x;
          far.y = y;
          break;
        }
      }
      break;
    case 1:
      while(x < 3){
        int step = 0;
        x++;
        step++;
        if(b[x][y]!=0 && step > max){
          max = step;
          far.x = x;
          far.y = y;
          break;
        }
      }
      break;
    case 2:
      while(y >= 1){
        int step = 0;
        y--;
        step++;
        if(b[x][y]!=0 && step > max){
          max = step;
          far.x = x;
          far.y = y;
          break;
        }
      }
      break;
    case 3:
      while(x >= 3){
        int step = 0;
        y++;
        step++;
        if(b[x][y]!=0 && step > max){
          max = step;
          far.x = x;
          far.y = y;
          break;
        }
      }
      break;
    }
  return far;
}

