#ifndef threes_IO_threes_IO_h
#define threes_IO_threes_IO_h
#include <threes_Mechanics.h>

/*
 *******************************************************************************
 * function declaration *
 *******************************************************************************
*/

/* Read board from file */
void readInFile(Board *, std::vector<int> *, char *);

/* Print board to stdout */
void printBoard(const Board &);

#endif