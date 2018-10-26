#include <iostream>
#include <vector>
#include <set>

void update_board(std::vector<std::vector<int>>& board, const int old_i, const int old_j, const int new_i, const int new_j, int& aux)
{
  aux = board[new_i][new_j];
  board[new_i][new_j] = board[old_i][old_j];
  board[old_i][old_j] = 0;
}

void reverse_update(std::vector<std::vector<int>>& board, const int old_i, const int old_j, const int new_i, const int new_j, const int aux)
{
  board[old_i][old_j] = board[new_i][new_j];
  board[new_i][new_j] = aux;
}

void fill_board(std::vector<std::vector<int>>& board, const int rows, const int columns, int& count)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      std::cin >> board[i][j];
      if(board[i][j] > 0)
        count++;
    }
  }
}

void fill_solutions(const std::vector<std::vector<int>>& board, std::set<std::vector<int>>& solutions)
{
  for(int i = 0; i < (int) board.size(); i++)
  {
    for(int j = 0; j < (int) board[j].size(); j++)
    {
      if(board[i][j] != 0)
        solutions.insert({i+1, j+1, board[i][j]});
    }
  }
}

void print_solutions(const std::set<std::vector<int>>& solutions, int total_sol)
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

bool is_valid_up(const std::vector<std::vector<int>>& board, const int i, const int j)
{
  if(i-1 >= 0)
    return (board[i-1][j] == board[i][j] + 1 || (board[i][j] == 3 && board[i-1][j] == 1));
  else
    return false;
}

bool is_valid_right(const std::vector<std::vector<int>>& board, const int i, const int j)
{
  if(j+1 < (int) board[i].size())
    return (board[i][j+1] == board[i][j] + 1 || (board[i][j] == 3 && board[i][j+1] == 1));
  else
    return false;
}

bool is_valid_down(const std::vector<std::vector<int>>& board, const int i, const int j)
{
  if(i+1 < (int) board.size())
    return (board[i+1][j] == board[i][j] + 1 || (board[i][j] == 3 && board[i+1][j] == 1));
  else
    return false;
}

bool is_valid_left(const std::vector<std::vector<int>>& board, const int i, const int j)
{
  if(j-1 >= 0)
    return (board[i][j-1] == board[i][j] + 1 || (board[i][j] == 3 && board[i][j-1] == 1));
  else
    return false;
}

void find_solutions(std::vector<std::vector<int>> board, int count, const int i, const int j, int& total_sol, std::set<std::vector<int>>& solutions)
{
  if(count == 1)
  {
    total_sol++;
    fill_solutions(board, solutions);
    return;
  }
  if(board[i][j] != 0)
  {
    if(is_valid_up(board, i, j))
    {
      int aux;
      update_board(board, i, j, i-1, j, aux);
      for(int _i = 0; _i < (int) board.size(); _i++)
      {
        for(int _j = 0; _j < (int) board[_i].size(); _j++)
        {
          find_solutions(board, count-1, _i, _j, total_sol, solutions);
          if(count == 2)
            //break the nested loop when the returned recursion was the last one
            goto jump_loop1;
        }
      }
jump_loop1:
      reverse_update(board, i, j, i-1, j, aux);
    }
    if(is_valid_right(board, i, j))
    {
      int aux;
      update_board(board, i, j, i, j+1, aux);
      for(int _i = 0; _i < (int) board.size(); _i++)
      {
        for(int _j = 0; _j < (int) board[_i].size(); _j++)
        {
          find_solutions(board, count-1, _i, _j, total_sol, solutions);
          if(count == 2)
            //break the nested loop when the returned recursion was the last one
            goto jump_loop2;
        }
      }
jump_loop2:
      reverse_update(board, i, j, i, j+1, aux);
    }
    if(is_valid_down(board, i, j))
    {
      int aux;
      update_board(board, i, j, i+1, j, aux);
      for(int _i = 0; _i < (int) board.size(); _i++)
      {
        for(int _j = 0; _j < (int) board[_i].size(); _j++)
        {
          find_solutions(board, count-1, _i, _j, total_sol, solutions);
          if(count == 2)
            //break the nested loop when the returned recursion was the last one
            goto jump_loop3;
        }
      }
jump_loop3:
      reverse_update(board, i, j, i+1, j, aux);
    }
    if(is_valid_left(board, i, j))
    {
      int aux;
      update_board(board, i, j, i, j-1, aux);
      for(int _i = 0; _i < (int) board.size(); _i++)
      {
        for(int _j = 0; _j < (int) board[_i].size(); _j++)
        {
          find_solutions(board, count-1, _i, _j, total_sol, solutions);
          if(count == 2)
            //break the nested loop when the returned recursion was the last one
            goto jump_loop4;
        }
      }
jump_loop4:
      reverse_update(board, i, j, i, j-1, aux);
    }
  }
}

int main()
{
  int rows, columns;
  std::cin >> rows >> columns;

  std::vector<std::vector<int>> board(rows, std::vector<int>(columns));
  int count = 0;
  fill_board(board, rows, columns, count);

  int total_sol = 0;
  std::set<std::vector<int>> solutions;
  for(int i = 0; i < (int) board.size(); i++)
  {
    for(int j = 0; j < (int) board[i].size(); j++)
    {
      find_solutions(board, count, i, j, total_sol, solutions);
    }
  }

  print_solutions(solutions, total_sol);

  return 0;
}
