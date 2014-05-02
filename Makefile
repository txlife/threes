CXX = clang++ 
CXXFLAGS = -std=c++11 -g -O -Wall -Weffc++
LDFLAGS = -lstdc++

SRC_DIR = ./src/
INC_DIR = ./include/

PROJ = threes
_SRC = $(PROJ).cpp
_INCLUDE = $(PROJ).h
SRC = $(addprefix $(SRC_DIR), $(_SRC))
INCLUDE = $(addprefix $(INC_DIR), $(_INCLUDE))
_OBJ = $(addsuffix .o, $(basename $(SRC)))
OBJ = $(addprefix $(OBJ_DIR), $(_OBJ))

INC_FLAGS = -I$(INC_DIR)

$(PROJ): $(OBJ)
	$(CXX) $(LDFLAGS) -o $(PROJ) $^

$(OBJ): $(SRC) $(INCLUDE)
	$(CXX) -c $(CXXFLAGS) -o $@ $(INC_FLAGS) $<

clean:
	rm $(PROJ) *.o

print_vars:
	echo $(OBJ)