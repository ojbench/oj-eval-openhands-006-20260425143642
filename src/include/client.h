#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>

extern int rows;         // The count of rows of the game map.
extern int columns;      // The count of columns of the game map.
extern int total_mines;  // The count of mines of the game map.

// You MUST NOT use any other external variables except for rows, columns and total_mines.

/**
 * @brief The definition of function Execute(int, int, bool)
 *
 * @details This function is designed to take a step when player the client's (or player's) role, and the implementation
 * of it has been finished by TA. (I hope my comments in code would be easy to understand T_T) If you do not understand
 * the contents, please ask TA for help immediately!!!
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 * @param type The type of operation to a certain block.
 * If type == 0, we'll execute VisitBlock(row, column).
 * If type == 1, we'll execute MarkMine(row, column).
 * If type == 2, we'll execute AutoExplore(row, column).
 * You should not call this function with other type values.
 */
void Execute(int r, int c, int type);

char client_map[32][32];
bool client_visited[32][32];
bool client_marked[32][32];

/**
 * @brief The definition of function InitGame()
 */
void InitGame() {
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      client_map[i][j] = '?';
      client_visited[i][j] = false;
      client_marked[i][j] = false;
    }
  }
  int first_row, first_column;
  if (!(std::cin >> first_row >> first_column)) return;
  Execute(first_row, first_column, 0);
}

/**
 * @brief The definition of function ReadMap()
 */
void ReadMap() {
  for (int i = 0; i < rows; ++i) {
    std::string line;
    if (!(std::cin >> line)) break;
    for (int j = 0; j < columns; ++j) {
      client_map[i][j] = line[j];
      if (line[j] >= '0' && line[j] <= '8') {
        client_visited[i][j] = true;
        client_marked[i][j] = false;
      } else if (line[j] == '@') {
        client_visited[i][j] = false;
        client_marked[i][j] = true;
      } else if (line[j] == 'X') {
        client_visited[i][j] = true;
        client_marked[i][j] = false;
      } else {
        client_visited[i][j] = false;
        client_marked[i][j] = false;
      }
    }
  }
}

/**
 * @brief The definition of function Decide()
 */
void Decide() {
  int total_unvisited = 0;
  int total_marked = 0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (client_marked[i][j]) total_marked++;
      else if (!client_visited[i][j]) total_unvisited++;
    }
  }

  if (total_unvisited > 0) {
    if (total_marked == total_mines) {
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
          if (!client_visited[i][j] && !client_marked[i][j]) {
            Execute(i, j, 0);
            return;
          }
        }
      }
    }
    if (total_unvisited + total_marked == total_mines) {
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
          if (!client_visited[i][j] && !client_marked[i][j]) {
            Execute(i, j, 1);
            return;
          }
        }
      }
    }
  }

  // 1. Obvious moves
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (client_visited[i][j] && client_map[i][j] >= '0' && client_map[i][j] <= '8') {
        int v = client_map[i][j] - '0';
        int U = 0, M = 0;
        for (int di = -1; di <= 1; ++di) {
          for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;
            int ni = i + di, nj = j + dj;
            if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
              if (client_marked[ni][nj]) M++;
              else if (!client_visited[ni][nj]) U++;
            }
          }
        }
        if (U > 0) {
          if (M == v) {
            Execute(i, j, 2); // Auto explore
            return;
          }
          if (M + U == v) {
            for (int di = -1; di <= 1; ++di) {
              for (int dj = -1; dj <= 1; ++dj) {
                if (di == 0 && dj == 0) continue;
                int ni = i + di, nj = j + dj;
                if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && !client_visited[ni][nj] && !client_marked[ni][nj]) {
                  Execute(ni, nj, 1); // Mark mine
                  return;
                }
              }
            }
          }
        }
      }
    }
  }

  // 2. Generalized subset reasoning
  for (int i1 = 0; i1 < rows; ++i1) {
    for (int j1 = 0; j1 < columns; ++j1) {
      if (client_visited[i1][j1] && client_map[i1][j1] >= '0' && client_map[i1][j1] <= '8') {
        int v1 = client_map[i1][j1] - '0';
        int M1 = 0;
        std::vector<std::pair<int, int>> U1;
        for (int di = -1; di <= 1; ++di) {
          for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;
            int ni = i1 + di, nj = j1 + dj;
            if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
              if (client_marked[ni][nj]) M1++;
              else if (!client_visited[ni][nj]) U1.push_back({ni, nj});
            }
          }
        }
        if (U1.empty()) continue;
        int R1 = v1 - M1;

        for (int i2 = std::max(0, i1 - 2); i2 <= std::min(rows - 1, i1 + 2); ++i2) {
          for (int j2 = std::max(0, j1 - 2); j2 <= std::min(columns - 1, j1 + 2); ++j2) {
            if (i1 == i2 && j1 == j2) continue;
            if (client_visited[i2][j2] && client_map[i2][j2] >= '0' && client_map[i2][j2] <= '8') {
              int v2 = client_map[i2][j2] - '0';
              int M2 = 0;
              std::vector<std::pair<int, int>> U2;
              for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                  if (di == 0 && dj == 0) continue;
                  int ni = i2 + di, nj = j2 + dj;
                  if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
                    if (client_marked[ni][nj]) M2++;
                    else if (!client_visited[ni][nj]) U2.push_back({ni, nj});
                  }
                }
              }
              if (U2.empty()) continue;
              int R2 = v2 - M2;

              std::vector<std::pair<int, int>> I, A, B;
              for (auto &p1 : U1) {
                bool in_U2 = false;
                for (auto &p2 : U2) {
                  if (p1.first == p2.first && p1.second == p2.second) {
                    in_U2 = true;
                    break;
                  }
                }
                if (in_U2) I.push_back(p1);
                else A.push_back(p1);
              }
              if (I.empty()) continue;
              for (auto &p2 : U2) {
                bool in_U1 = false;
                for (auto &p1 : U1) {
                  if (p1.first == p2.first && p1.second == p2.second) {
                    in_U1 = true;
                    break;
                  }
                }
                if (!in_U1) B.push_back(p2);
              }

              if (R2 - R1 == (int)B.size()) {
                if (!B.empty()) { Execute(B[0].first, B[0].second, 1); return; }
                if (!A.empty()) { Execute(A[0].first, A[0].second, 0); return; }
              }
              if (R1 - R2 == (int)A.size()) {
                if (!A.empty()) { Execute(A[0].first, A[0].second, 1); return; }
                if (!B.empty()) { Execute(B[0].first, B[0].second, 0); return; }
              }
            }
          }
        }
      }
    }
  }

  // 3. Guess
  int best_r = -1, best_c = -1;
  double min_prob = 2.0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (!client_visited[i][j] && !client_marked[i][j]) {
        double prob = (total_unvisited > 0) ? (double)(total_mines - total_marked) / total_unvisited : 1.0;
        bool near_visited = false;
        for (int di = -1; di <= 1; ++di) {
          for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;
            int ni = i + di, nj = j + dj;
            if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && client_visited[ni][nj]) {
              near_visited = true;
              int v = client_map[ni][nj] - '0';
              int M = 0, U = 0;
              for (int ddi = -1; ddi <= 1; ++ddi) {
                for (int ddj = -1; ddj <= 1; ++ddj) {
                  if (ddi == 0 && ddj == 0) continue;
                  int nni = ni + ddi, nnj = nj + ddj;
                  if (nni >= 0 && nni < rows && nnj >= 0 && nnj < columns) {
                    if (client_marked[nni][nnj]) M++;
                    else if (!client_visited[nni][nnj]) U++;
                  }
                }
              }
              if (U > 0) {
                double p = (double)(v - M) / U;
                if (p < prob) prob = p;
              }
            }
          }
        }
        if (prob < min_prob) { min_prob = prob; best_r = i; best_c = j; }
      }
    }
  }
  if (best_r != -1) Execute(best_r, best_c, 0);
}

#endif