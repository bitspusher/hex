#pragma once

#include "hex_board.h"

// Forward declarations
class HexCell;
class HexBoard;
enum class HexPlayer;

/* Hex Solver based on Monte Carlo method */
class HexMonteCarlo {
private:
  // Member variables
  int numTrials_ = 1000;
  const HexBoard *input_board_;
  HexPlayer ai_{};
  HexPlayer user_{};

  // Member functions
  void fillBoardRandomly(HexBoard &board);
  double getProbabilityOfWin(HexCell move);

public:
  /* Constructor. Evaluates best move based on the current state of a provided
   * hex board*/
  HexMonteCarlo(const HexBoard *board, const HexPlayer ai,
                const HexPlayer user);

  /* Runs a Monte Carlo simulation to find the move that has the highest
   * probability of resulting in a win*/
  HexCell getBestMove(void);
};
