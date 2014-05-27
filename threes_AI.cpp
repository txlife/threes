#include <threes_AI.h>

void hillClimb(Board &board) {
  PQ moveQueue;
  
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
std::vector<Node> dfs(Board &board) {
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
  std::cout << max_score << "\n";
  return parents;
}
/*
int greedy_search(Board board, int depth, int tile) {
  if (depth == 0) return score(board);
  std::vector<Direction> poss_moves = getPossibleMoves(board, tile);
  if (poss_moves.size() == 0) return score(board);

  int best_val = -1;
  for (Direction m : poss_moves) {
    std::vector< std::vector<int> > b_copy = board;
    std::vector<Shift> shifts = makeMove(&b_copy, m, tile); // possibly add shifts # to eval total
    best_val = std::max(best_val, greedy_search(&b_copy, depth - 1, (tile + 1) % (inputSequence.size() - 1)));
  }
  return best_val;
}
*/
/*
int dls(Board board, int tile){
  int i = 0;
  std::vector<Direction> poss_moves = getPossibleMoves(board,tile);

}

int a_search(start,goal){
  std::set<Node1> openset; 
  openset.insert(start);
  std::set<Node1> closedset; 
  came_from := empty map;
  g_score[start]=0;
  h_score[start]=0;
  f_score[start]=0;
  while(!openset.empty()){
    Node1 x;//the node in openset having highest fscore;
    if(x==goal){
      return reconstruct_path(came_from,goal);
    }
    std::vector<Direction> poss_moves = getPossibleMoves(board,tile);
    openset.earse(x);
    closedset.add(x);
    for (Direction m : poss_moves) {
      Node1 y = Node1(m,x);
      std::vector< std::vector<int> > b_copy = board;
      std::vector<Shift> shifts = makeMove(&b_copy, d, tile);
      //const bool is_in = closedset.find(y) != closedset.end();
      if (closedset.find(y) != closedset.end()){
        continue;
      }
      tentative_g_score := x.g_score + score(b_copy);

      if(openset.find(y) == openset.end()){
        openset.insert(y);
        tentative_is_better := true;
      }
      else if(tentative_g_score > y.g_score){
        tentative_is_better;
      }
      else{
        tentative_is_better := false;
      }
      if(tentative_is_better == true){
        came_from[ynode] := x;
        g_score[y] := tentative_g_score;
        h_score[y] := heuristic_estimate_of_distance(y,goal);
        f_score[y] := g_score[y] + h_score[y];
      }
    }
  }
  return failure;
}

Node Function reconstruct_path(came_from, current_node){
  if came_from[current_node] is set{
    p = reconstruct_path(came_from,came_from[current_node]);
    return (p+current_node);
  }
  else{
    return current_node;
  }
}
*/


Direction greedy_search(Board board, int tile){
  std::vector<Direction> poss_moves = getPossibleMoves(board, tile);
  int sss = 0;
  int tile1 = tile + 1;
  int tile2 = tile1 + 1;
  int tile3 = tile2 + 1;
  int tile4 = tile3 + 1;
  int tile5 = tile4 + 1;
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
              if(score(g_copy)>sss){
                sss = score(g_copy);
                ddd = m;
              }
            }
          }
        }
      }
    }
  }
  return ddd;
}
