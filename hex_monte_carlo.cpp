
#include "hex_monte_carlo.h"
#include "hex_board.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <unordered_map>

HexMonteCarlo::HexMonteCarlo(const HexBoard *board, const HexPlayer ai,
                             const HexPlayer user)
    : input_board_(board), ai_(ai), user_(user) {}

void HexMonteCarlo::fillBoardRandomly(HexBoard &board) {
  std::vector<HexCell> availableCells;
  HexPlayer currentPlayer = ai_;

  for (auto &row : board.rows_) {
    for (auto &cell : row) {
      if (cell.isEmpty()) {
        availableCells.push_back(cell);
      }
    }
  }
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(availableCells.begin(), availableCells.end(),
               std::default_random_engine(seed));

  for (auto &cell : availableCells) {
    board.makeMove(cell.getRow(), cell.getCol(), currentPlayer);
    currentPlayer =
        (currentPlayer == HexPlayer::Blue) ? HexPlayer::Red : HexPlayer::Blue;
  }
}

double HexMonteCarlo::getProbabilityOfWin(HexCell move) {
  uint32_t wins = 0;

  for (auto trial = 0; trial < numTrials_; trial++) {
    HexBoard tempBoard = *input_board_;
    tempBoard.makeMove(move.getRow(), move.getCol(), ai_);
    fillBoardRandomly(tempBoard);

    std::pair<bool, HexPlayer> gameStatus = tempBoard.checkIfGameHasEnded();
    auto result = gameStatus.first;
    auto winner = gameStatus.second;

    assert(result == true);

    if (winner == ai_) {
      wins++;
    }
  }
  return (1.0 * wins) / numTrials_;
}

HexCell HexMonteCarlo::getBestMove(void) {
  std::vector<HexCell> availableMoves{};

  for (auto &row : input_board_->rows_) {
    for (auto &cell : row) {
      if (cell.isEmpty()) {
        availableMoves.push_back(cell);
      }
    }
  }

  std::unordered_map<HexCell *, double> win_probabilities;
  for (auto &move : availableMoves) {
    win_probabilities[&move] = getProbabilityOfWin(move);
  }

  HexCell *max_win_prob_cell;
  double max_win_prob = 0.0;

  for (auto &prob : win_probabilities) {
    if (prob.second > max_win_prob) {
      max_win_prob_cell = prob.first;
      max_win_prob = prob.second;
    }
  }
  return *max_win_prob_cell;
}
