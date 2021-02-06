COMPILER := g++
FLAGS    := -Ofast -std=c++11
SRC      := hex.cpp hex_board.cpp hex_monte_carlo.cpp
OUTPUT   := hex
all:
	$(COMPILER) $(SRC) $(FLAGS) -o $(OUTPUT)
clean:
	rm -rf $(OUTPUT)
