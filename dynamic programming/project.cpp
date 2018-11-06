#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <algorithm>

#define EMPTY 0
#define ROCK 1
#define SCISSOR 2
#define PAPER 3

using namespace std;

// map where the key is a 64 bit integer (the board) and the value is the number of solutions of that board
// each ordered pair of 2 bits represent 1 value of a given position in the 2D board
typedef unordered_map<uint64_t, unsigned long long> u_map_uint64;

// size of the board
int rows, columns;

// return the value stored on position i and j (2D) on the board (1D)
int get_value(const uint64_t& board, const int i, const int j)
{
  int pos = i*columns + j;
  int value = (board >> 2*pos) & 3;
  return value;
}

// store the 'value' on the board
void set_value(uint64_t& board, const int i, const int j, const int value)
{
  int pos = i*columns + j;
  // clear the two bits where the value will be stored
  board &= ~(uint64_t(3) << 2*pos);
  // store the value
  board |= (uint64_t(value) << 2*pos);
}

int update_board(uint64_t& board, const int old_i, const int old_j, const int new_i, const int new_j)
{
  int new_value = get_value(board, new_i, new_j);
  int old_value = get_value(board, old_i, old_j);
  int stored_value = new_value;
  set_value(board, new_i, new_j, old_value);
  set_value(board, old_i, old_j, EMPTY);
  return stored_value;
}

void reverse_update(uint64_t& board, const int old_i, const int old_j, const int new_i, const int new_j, const int stored_value)
{
  int new_value = get_value(board, new_i, new_j);
  set_value(board, old_i, old_j, new_value);
  set_value(board, new_i, new_j, stored_value);
}

void fill_board(uint64_t& board, unsigned& count)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      int value;
      cin >> value;
      set_value(board, i, j, value);
      if(value > EMPTY)
        count++;
    }
  }
}

void fill_solutions(const uint64_t& board, vector<tuple<int, int, int>>& solutions)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      int value = get_value(board, i, j);
      if(value != EMPTY)
        solutions.emplace_back(i+1, j+1, value);
    }
  }
}

void print_solutions(const vector<tuple<int, int, int>>& solutions, const unsigned long long total_solutions)
{
  cout << total_solutions << "\n"
       << solutions.size() << "\n";
  for(auto s : solutions)
  {
    cout << get<0>(s) << " " << get<1>(s) << " " << get<2>(s) << "\n";
  }
}

// Do a depth-first search on the board
void dfs(const uint64_t& board, const int i, const int j, vector<vector<bool>>& visited)
{
  if(i < 0 || i > rows-1 || j < 0 || j > columns-1)
    return;

  if(visited[i][j] == true)
    return;

  if(get_value(board, i, j) != EMPTY)
  {
    visited[i][j] = true;
    dfs(board, i-1, j, visited);
    dfs(board, i+1, j, visited);
    dfs(board, i, j-1, visited);
    dfs(board, i, j+1, visited);
  }
}

// check if all the pieces of the board can be achieved by a starting piece
bool not_solvable(const uint64_t& board)
{
  // initialize the visited vector to false on all positions
  vector<vector<bool>> visited(rows, vector<bool>(columns, false));

  // find the first piece of the board and call dfs() for that piece
  int i, j;
  bool outer_loop = true;
  for(i = 0; i < rows && outer_loop; i++)
  {
    for(j = 0; j < columns; j++)
    {
      if(get_value(board, i, j) != EMPTY)
      {
        dfs(board, i, j, visited);
        outer_loop = false;
        break;
      }
    }
  }

  // starting on the first piece, check if all the pieces on the board were visited
  for(int _i = i; _i < rows; _i++)
  {
    for(int _j = j; _j < columns; _j++)
    {
      if(get_value(board, _i, _j) != EMPTY)
      {
        if(visited[_i][_j] == false)
          // at least one piece weren't visited
          return true;
      }
    }
  }

  // all pieces were visited
  return false;
}

bool is_valid_up(const uint64_t& board, const int i, const int j)
{
  if(i-1 >= 0)
  {
    int up_pos = get_value(board, i-1, j);
    int current_pos = get_value(board, i, j);
    return (up_pos == current_pos + 1 || (current_pos == PAPER && up_pos == ROCK));
  }
  else
    return false;
}

bool is_valid_right(const uint64_t& board, const int i, const int j)
{
  if(j+1 < columns)
  {
    int right_pos = get_value(board, i, j+1);
    int current_pos = get_value(board, i, j);
    return (right_pos == current_pos + 1 || (current_pos == PAPER && right_pos == ROCK));
  }
  else
    return false;
}

bool is_valid_down(const uint64_t& board, const int i, const int j)
{
  if(i+1 < rows)
  {
    int down_pos = get_value(board, i+1, j);
    int current_pos = get_value(board, i, j);
    return (down_pos == current_pos + 1 || (current_pos == PAPER && down_pos == ROCK));
  }
  else
    return false;
}

bool is_valid_left(const uint64_t& board, const int i, const int j)
{
  if(j-1 >= 0)
  {
    int left_pos = get_value(board, i, j-1);
    int current_pos = get_value(board, i, j);
    return (left_pos == current_pos + 1 || (current_pos == PAPER && left_pos == ROCK));
  }
  else
    return false;
}

unsigned long long find_solutions(uint64_t board, vector<tuple<int, int, int>>& solutions, const unsigned count)
{
  static u_map_uint64 memoization;

  // check if the number of solutions for this board is already stored
  const auto search = memoization.find(board);
  if(search != memoization.end())
  {
    return search->second;
  }

  // if there's only one piece left on the board
  if(count == 1)
  {
    fill_solutions(board, solutions);
    memoization[board] = 1;
    return 1;
  }

  if(not_solvable(board))
  {
    memoization[board] = 0;
    return 0;
  }

  unsigned long long total_solutions = 0;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      if(get_value(board, i, j) != EMPTY)
      {
        if(is_valid_up(board, i, j))
        {
          int stored_value = update_board(board, i, j, i-1, j);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i-1, j, stored_value);
        }
        if(is_valid_right(board, i, j))
        {
          int stored_value = update_board(board, i, j, i, j+1);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i, j+1, stored_value);
        }
        if(is_valid_down(board, i, j))
        {
          int stored_value = update_board(board, i, j, i+1, j);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i+1, j, stored_value);
        }
        if(is_valid_left(board, i, j))
        {
          int stored_value = update_board(board, i, j, i, j-1);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i, j-1, stored_value);
        }
      }
    }
  }

  memoization[board] = total_solutions;
  return total_solutions;
}

int main()
{
  cin >> rows >> columns;

  uint64_t board;

  // number of pieces on the board
  unsigned count = 0;
  fill_board(board, count);

  vector<tuple<int, int, int>> solutions;
  unsigned long long total_solutions = find_solutions(board, solutions, count);

  sort(solutions.begin(), solutions.end());
  print_solutions(solutions, total_solutions);

  return 0;
}
