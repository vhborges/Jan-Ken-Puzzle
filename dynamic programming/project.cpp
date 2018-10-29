#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <bitset>

typedef std::set<std::vector<int>> set_vector_int;
typedef std::bitset<50> bitset_50;
typedef std::unordered_map<bitset_50, unsigned> u_map_bitset_unsigned;

int rows, columns;

int get_value(const bitset_50& board, const int i, const int j)
{
  int pos = i*columns + j;
  int value = ((board >> 2*pos) & bitset_50(3)).to_ulong();
  return value;
}

void set_value(bitset_50& board, const int i, const int j, int value)
{
  int pos = i*columns + j;
  board.reset(2*pos);
  board.reset(2*pos+1);
  bitset_50 value_as_bitset(value);
  board |= (value_as_bitset << 2*pos);
}

int update_board(bitset_50& board, const int old_i, const int old_j, const int new_i, const int new_j)
{
  int new_value = get_value(board, new_i, new_j);
  int old_value = get_value(board, old_i, old_j);
  int aux = new_value;
  set_value(board, new_i, new_j, old_value);
  set_value(board, old_i, old_j, 0);
  return aux;
}

void reverse_update(bitset_50& board, const int old_i, const int old_j, const int new_i, const int new_j, const int aux)
{
  int new_value = get_value(board, new_i, new_j);
  set_value(board, old_i, old_j, new_value);
  set_value(board, new_i, new_j, aux);
}

void fill_board(bitset_50& board, unsigned& count)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      int value;
      std::cin >> value;
      set_value(board, i, j, value);
      if(value > 0)
        count++;
    }
  }
}

void fill_solutions(const bitset_50& board, set_vector_int& solutions)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      int value = get_value(board, i, j);
      if(value != 0)
        solutions.insert({i+1, j+1, value});
    }
  }
}

void print_solutions(const set_vector_int& solutions, const int total_sol)
{
  std::cout << total_sol << "\n" << solutions.size() << "\n";
  for(auto s : solutions)
  {
    for(auto v : s)
    {
      std::cout << v << " ";
    }
    std::cout << "\n";
  }
}

bool is_valid_up(const bitset_50& board, const int i, const int j)
{
  if(i-1 >= 0)
  {
    int board_up = get_value(board, i-1, j);
    int board_actual = get_value(board, i, j);
    return (board_up == board_actual + 1 || (board_actual == 3 && board_up == 1));
  }
  else
    return false;
}

bool is_valid_right(const bitset_50& board, const int i, const int j)
{
  if(j+1 < columns)
  {
    int board_right = get_value(board, i, j+1);
    int board_actual = get_value(board, i, j);
    return (board_right == board_actual + 1 || (board_actual == 3 && board_right == 1));
  }
  else
    return false;
}

bool is_valid_down(const bitset_50& board, const int i, const int j)
{
  if(i+1 < rows)
  {
    int board_down = get_value(board, i+1, j);
    int board_actual = get_value(board, i, j);
    return (board_down == board_actual + 1 || (board_actual == 3 && board_down == 1));
  }
  else
    return false;
}

bool is_valid_left(const bitset_50& board, const int i, const int j)
{
  if(j-1 >= 0)
  {
    int board_left = get_value(board, i, j-1);
    int board_actual = get_value(board, i, j);
    return (board_left == board_actual + 1 || (board_actual == 3 && board_left == 1));
  }
  else
    return false;
}

unsigned long find_solutions(bitset_50 board, set_vector_int& solutions, const unsigned count)
{
  static u_map_bitset_unsigned memoization;

  const auto search = memoization.find(board);
  if(search != memoization.end())
  {
    return search->second;
  }

  if(count == 1)
  {
    fill_solutions(board, solutions);
    memoization[board] = 1;
    return 1;
  }

  unsigned long total_solutions = 0;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      if(get_value(board, i, j) != 0)
      {
        if(is_valid_up(board, i, j))
        {
          int old_value = update_board(board, i, j, i-1, j);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i-1, j, old_value);
        }
        if(is_valid_right(board, i, j))
        {
          int old_value = update_board(board, i, j, i, j+1);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i, j+1, old_value);
        }
        if(is_valid_down(board, i, j))
        {
          int old_value = update_board(board, i, j, i+1, j);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i+1, j, old_value);
        }
        if(is_valid_left(board, i, j))
        {
          int old_value = update_board(board, i, j, i, j-1);
          total_solutions += find_solutions(board, solutions, count-1);
          reverse_update(board, i, j, i, j-1, old_value);
        }
      }
    }
  }

  memoization[board] = total_solutions;
  return total_solutions;
}


//void find_solutions(bitset_50 board, u_map_bitset_unsigned& memoization, unsigned int count, const int i, const int j, set_vector_int& solutions)
//{
//  if(count == 1)
//  {
//    fill_solutions(board, solutions);
//    memoization[board] = 1;
//    return;
//  }
//  const auto search = memoization.find(board);
//  if(search != memoization.end())
//  {
//    memoization[board] += search->second;
//    return;
//  }
//    if(get_value(board, i, j) != 0)
//    {
//      if(is_valid_up(board, i, j))
//      {
//        int aux;
//        update_board(board, i, j, i-1, j, aux);
//        for(int _i = 0; _i < rows; _i++)
//        {
//          for(int _j = 0; _j < columns; _j++)
//          {
//            find_solutions(board, memoization, count-1, _i, _j, solutions);
//            if(count == 2)
//              //break the nested loop when the returned recursion was the last one
//              goto jump_loop1;
//          }
//        }
//jump_loop1:
//        if(memoization.find(board) == memoization.end())
//        {
//          memoization[board] = 0;
//          reverse_update(board, i, j, i-1, j, aux);
//        }
//        else
//        {
//          unsigned no_of_solutions = memoization[board];
//          reverse_update(board, i, j, i-1, j, aux);
//          memoization[board] += no_of_solutions;
//        }
//      }
//      if(is_valid_right(board, i, j))
//      {
//        int aux;
//        update_board(board, i, j, i, j+1, aux);
//        for(int _i = 0; _i < rows; _i++)
//        {
//          for(int _j = 0; _j < columns; _j++)
//          {
//            find_solutions(board, memoization, count-1, _i, _j, solutions);
//            if(count == 2)
//              //break the nested loop when the returned recursion was the last one
//              goto jump_loop2;
//          }
//        }
//jump_loop2:
//        if(memoization.find(board) == memoization.end())
//        {
//          memoization[board] = 0;
//          reverse_update(board, i, j, i, j+1, aux);
//        }
//        else
//        {
//          unsigned no_of_solutions = memoization[board];
//          reverse_update(board, i, j, i, j+1, aux);
//          memoization[board] += no_of_solutions;
//        }
//      }
//      if(is_valid_down(board, i, j))
//      {
//        int aux;
//        update_board(board, i, j, i+1, j, aux);
//        for(int _i = 0; _i < rows; _i++)
//        {
//          for(int _j = 0; _j < columns; _j++)
//          {
//            find_solutions(board, memoization, count-1, _i, _j, solutions);
//            if(count == 2)
//              //break the nested loop when the returned recursion was the last one
//              goto jump_loop3;
//          }
//        }
//jump_loop3:
//        if(memoization.find(board) == memoization.end())
//        {
//          memoization[board] = 0;
//          reverse_update(board, i, j, i+1, j, aux);
//        }
//        else
//        {
//          unsigned no_of_solutions = memoization[board];
//          reverse_update(board, i, j, i+1, j, aux);
//          memoization[board] += no_of_solutions;
//        }
//      }
//      if(is_valid_left(board, i, j))
//      {
//        int aux;
//        update_board(board, i, j, i, j-1, aux);
//        for(int _i = 0; _i < rows; _i++)
//        {
//          for(int _j = 0; _j < columns; _j++)
//          {
//            find_solutions(board, memoization, count-1, _i, _j, solutions);
//            if(count == 2)
//              //break the nested loop when the returned recursion was the last one
//              goto jump_loop4;
//          }
//        }
//jump_loop4:
//        if(memoization.find(board) == memoization.end())
//        {
//          memoization[board] = 0;
//          reverse_update(board, i, j, i, j-1, aux);
//        }
//        else
//        {
//          unsigned no_of_solutions = memoization[board];
//          reverse_update(board, i, j, i, j-1, aux);
//          memoization[board] += no_of_solutions;
//        }
//      }
//    }
//}

int main()
{
  std::cin >> rows >> columns;

  bitset_50 board;
  unsigned count = 0;
  fill_board(board, count);

  set_vector_int solutions;
  unsigned long total_solutions = find_solutions(board, solutions, count);

  print_solutions(solutions, total_solutions);

  return 0;
}
