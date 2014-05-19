#include <threes_AI.h>

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

int greedy_search(Board &board, int depth, int tile) {
  if (depth == 0) return score(board);
  std::vector<Direction> poss_moves = getPossibleMoves(board, tile);
  if (poss_moves.size() == 0) return score(board);
  int best_val = -1;
  for (Direction m : poss_moves) {
    Board b = board;
    makeMove(&b, m, tile); // possibly add shifts # to eval total
    best_val = std::max(best_val, greedy_search(b, depth - 1, (tile + 1) % (inputSequence.size() - 1)));
  }
  return best_val;
}
