#include <threes_IO.h>

/**
 * Read in the initial board values and input sequence for game of Threes!
 * @param board          Board container to initialize
 * @param inputSequence Game input sequence of tiles
 * @param file_name      File name for the input file
 */
void readInFile(Board *board, std::vector<int> *inputSequence, char *file_name) {
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
        inputSequence->push_back(std::stoi(string_val));
      }
    }

  } else {
    std::cout << "Could not open file: \"" << file_name << "\"\n";
    std::exit(EXIT_FAILURE);
  }
}

/* Print board to stdout */
void printBoard(const Board &board) {
  for (std::vector<int> l : board) {
    for (int c : l)
      std::cout << std::setfill(' ') << std::setw(4) << c << " ";
    std::cout << "\n";
  }
  std::cout << "\n";
}