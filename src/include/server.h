#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <string>

/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to use global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
int rows;         // The count of rows of the game map. You MUST NOT modify its name.
int columns;      // The count of columns of the game map. You MUST NOT modify its name.
int total_mines;  // The count of mines of the game map. You MUST NOT modify its name. You should initialize this
                  // variable in function InitMap. It will be used in the advanced task.
int game_state;  // The state of the game, 0 for continuing, 1 for winning, -1 for losing. You MUST NOT modify its name.

int grid_map[32][32]; // 0: non-mine, 1: mine
int grid_state[32][32]; // 0: unvisited, 1: visited, 2: marked
int grid_mine_count[32][32];
int visited_non_mine_count = 0;
int correctly_marked_mines = 0;

/**
 * @brief The definition of function InitMap()
 */
void InitMap() {
  if (!(std::cin >> rows >> columns)) return;
  total_mines = 0;
  visited_non_mine_count = 0;
  correctly_marked_mines = 0;
  game_state = 0;
  for (int i = 0; i < rows; ++i) {
    std::string line;
    std::cin >> line;
    for (int j = 0; j < columns; ++j) {
      grid_state[i][j] = 0;
      if (line[j] == 'X') {
        grid_map[i][j] = 1;
        total_mines++;
      } else {
        grid_map[i][j] = 0;
      }
    }
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (grid_map[i][j] == 0) {
        int count = 0;
        for (int di = -1; di <= 1; ++di) {
          for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;
            int ni = i + di, nj = j + dj;
            if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && grid_map[ni][nj] == 1) {
              count++;
            }
          }
        }
        grid_mine_count[i][j] = count;
      }
    }
  }
  if (visited_non_mine_count == rows * columns - total_mines) {
    game_state = 1;
  }
}

/**
 * @brief The definition of function VisitBlock(int, int)
 */
void VisitBlock(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= columns || grid_state[r][c] != 0 || game_state != 0) return;

  if (grid_map[r][c] == 1) {
    grid_state[r][c] = 1;
    game_state = -1;
  } else {
    grid_state[r][c] = 1;
    visited_non_mine_count++;
    if (visited_non_mine_count == rows * columns - total_mines) {
      game_state = 1;
      return;
    }
    if (grid_mine_count[r][c] == 0) {
      for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
          if (di == 0 && dj == 0) continue;
          VisitBlock(r + di, c + dj);
        }
      }
    }
  }
}

/**
 * @brief The definition of function MarkMine(int, int)
 */
void MarkMine(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= columns || grid_state[r][c] != 0 || game_state != 0) return;

  if (grid_map[r][c] == 1) {
    grid_state[r][c] = 2;
    correctly_marked_mines++;
  } else {
    grid_state[r][c] = 2;
    game_state = -1;
  }
}

/**
 * @brief The definition of function AutoExplore(int, int)
 */
void AutoExplore(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= columns || grid_state[r][c] != 1 || grid_map[r][c] == 1 || game_state != 0) return;
  int marked_count = 0;
  for (int di = -1; di <= 1; ++di) {
    for (int dj = -1; dj <= 1; ++dj) {
      if (di == 0 && dj == 0) continue;
      int ni = r + di, nj = c + dj;
      if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && grid_state[ni][nj] == 2) {
        marked_count++;
      }
    }
  }
  if (marked_count == grid_mine_count[r][c]) {
    for (int di = -1; di <= 1; ++di) {
      for (int dj = -1; dj <= 1; ++dj) {
        if (di == 0 && dj == 0) continue;
        int ni = r + di, nj = c + dj;
        if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && grid_map[ni][nj] == 0) {
          VisitBlock(ni, nj);
        }
      }
    }
  }
}

/**
 * @brief The definition of function ExitGame()
 */
void ExitGame() {
  if (game_state == 1) {
    std::cout << "YOU WIN!" << std::endl;
    std::cout << visited_non_mine_count << " " << total_mines << std::endl;
  } else {
    std::cout << "GAME OVER!" << std::endl;
    std::cout << visited_non_mine_count << " " << correctly_marked_mines << std::endl;
  }
  exit(0);
}

/**
 * @brief The definition of function PrintMap()
 */
void PrintMap() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (game_state == 1) {
        if (grid_map[i][j] == 1) std::cout << "@";
        else std::cout << grid_mine_count[i][j];
      } else {
        if (grid_state[i][j] == 0) std::cout << "?";
        else if (grid_state[i][j] == 1) {
          if (grid_map[i][j] == 1) std::cout << "X";
          else std::cout << grid_mine_count[i][j];
        } else if (grid_state[i][j] == 2) {
          if (grid_map[i][j] == 1) std::cout << "@";
          else std::cout << "X";
        }
      }
    }
    std::cout << std::endl;
  }
}

#endif
