CXX               = clang++
CXXFLAGS 					= -std=c++11 -g -O -Wall -Weffc++
LDFLAGS           = -lstdc++

SRC_DIR           = ./src/
INC_DIR           = ./include/
OBJ_DIR           = ./src/

INC_FLAGS         = -I$(INC_DIR)

PROJ							= threes

SRC_FILES 				= threes.cpp threes_AI.cpp threes_Mechanics.cpp threes_IO.cpp
OBJECTS					  = $(patsubst %.cpp, %.o, $(SRC_FILES))

all: $(PROJ)

$(PROJ): $(OBJECTS)
	$(CXX) -o $(PROJ) $(OBJECTS) $(LDLAGS)

%.o: %.cpp 
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(INC_FLAGS)

clean:
	rm $(PROJ) $(OBJECTS)
