#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <bitset>

#define EMPTY 0
#define ROCK 1
#define SCISSOR 2
#define PAPER 3

using namespace std;

// map where the key is a bitset of size 50 (the board) and the value is the number of solutions of that board
// each ordered pair of 2 bits represent 1 value of a given position in the 2D board
typedef unordered_map<bitset<50>, unsigned> u_map_bitset;

// size of the board
int rows, columns;

// return the value stored on position i and j (2D) on the board (1D)
int get_value(const bitset<50>& board, const int i, const int j)
{
  int pos = i*columns + j;
  int value = ((board >> 2*pos) & bitset<50>(3)).to_ulong();
  return value;
}

// store the 'value' on the board
void set_value(bitset<50>& board, const int i, const int j, const int value)
{
  int pos = i*columns + j;
  // clear the two bits on which the value will be stored
  board.reset(2*pos);
  board.reset(2*pos+1);
  bitset<50> value_as_bitset(value);
  board |= (value_as_bitset << 2*pos);
}

int update_board(bitset<50>& board, const int old_i, const int old_j, const int new_i, const int new_j)
{
  int new_value = get_value(board, new_i, new_j);
  int old_value = get_value(board, old_i, old_j);
  int stored_value = new_value;
  set_value(board, new_i, new_j, old_value);
  set_value(board, old_i, old_j, EMPTY);
  return stored_value;
}

void reverse_update(bitset<50>& board, const int old_i, const int old_j, const int new_i, const int new_j, const int stored_value)
{
  int new_value = get_value(board, new_i, new_j);
  set_value(board, old_i, old_j, new_value);
  set_value(board, new_i, new_j, stored_value);
}

void fill_board(bitset<50>& board, unsigned& count)
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

void fill_solutions(const bitset<50>& board, set<vector<int>>& solutions)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      int value = get_value(board, i, j);
      if(value != EMPTY)
        solutions.insert({i+1, j+1, value});
    }
  }
}

void print_solutions(const set<vector<int>>& solutions, const int total_sol)
{
  cout << total_sol << "\n" << solutions.size() << "\n";
  for(auto s : solutions)
  {
    for(auto v : s)
    {
      cout << v << " ";
    }
    cout << "\n";
  }
}

// Do a depth-first search on the board
void dfs(const bitset<50>& board, const int i, const int j, vector<vector<bool>>& visited)
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
bool is_solvable(const bitset<50>& board)
{
  // initialize the visited vector to false on all positions
  vector<vector<bool>> visited(rows, vector<bool>(columns, false));

  // find the first piece of the board and call dfs() for that piece
  int i, j;
  for(i = 0; i < rows; i++)
  {
    for(j = 0; j < columns; j++)
    {
      if(get_value(board, i, j) != EMPTY)
      {
        dfs(board, i, j, visited);
        goto jump_nested_loop;
      }
    }
  }
jump_nested_loop:

  // starting on the first piece, check if all the pieces on the board were visited
  for(int _i = i; _i < rows; _i++)
  {
    for(int _j = j; _j < columns; _j++)
    {
      if(get_value(board, _i, _j) != EMPTY)
      {
        if(visited[_i][_j] == false)
          // at least one piece weren't visited
          return false;
      }
    }
  }

  // all pieces were visited
  return true;
}

bool is_valid_up(const bitset<50>& board, const int i, const int j)
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

bool is_valid_right(const bitset<50>& board, const int i, const int j)
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

bool is_valid_down(const bitset<50>& board, const int i, const int j)
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

bool is_valid_left(const bitset<50>& board, const int i, const int j)
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

unsigned long long find_solutions(bitset<50> board, set<vector<int>>& solutions, const unsigned count)
{
  static u_map_bitset memoization;

  // check if the number of solutions for this board is already stored
  const auto search = memoization.find(board);
  if(search != memoization.end())
  {
    return search->second;
  }

  if(!is_solvable(board))
  {
    memoization[board] = 0;
    return 0;
  }

  // if there's only one piece left on the board
  if(count == 1)
  {
    fill_solutions(board, solutions);
    memoization[board] = 1;
    return 1;
  }

  unsigned long long total_solutions = 0;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      if(get_value(board, i, j) != 0)
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

  bitset<50> board;
  // number of pieces on the board
  unsigned count = 0;
  fill_board(board, count);

  set<vector<int>> solutions;
  unsigned long long total_solutions = find_solutions(board, solutions, count);

  print_solutions(solutions, total_solutions);

  return 0;
}
