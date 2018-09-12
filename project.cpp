#include <iostream>
#include <vector>

void print_solution(const std::vector<std::vector<int>>& board)
{
  for(auto i : board)
  {
    for(auto j : i)
    {
      std::cout << j << " ";
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

bool is_valid_up(const std::vector<std::vector<int>>& board, std::size_t i, std::size_t j)
{
  if((int) i-1 >= 0)
    return (board[i-1][j] == board[i][j] + 1 || (board[i][j] == 3 && board[i-1][j] == 1));
  else
    return false;
}

bool is_valid_right(const std::vector<std::vector<int>>& board, std::size_t i, std::size_t j)
{
  if(j+1 < board[i].size())
    return (board[i][j+1] == board[i][j] + 1 || (board[i][j] == 3 && board[i][j+1] == 1));
  else
    return false;
}

bool is_valid_down(const std::vector<std::vector<int>>& board, std::size_t i, std::size_t j)
{
  if(i+1 < board.size())
    return (board[i+1][j] == board[i][j] + 1 || (board[i][j] == 3 && board[i+1][j] == 1));
  else
    return false;
}

bool is_valid_left(const std::vector<std::vector<int>>& board, std::size_t i, std::size_t j)
{
  if((int) j-1 >= 0)
    return (board[i][j-1] == board[i][j] + 1 || (board[i][j] == 3 && board[i][j-1] == 1));
  else
    return false;
}

void find_solutions(std::vector<std::vector<int>> board, int count, std::size_t i, std::size_t j)
{
  if(count == 1)
  {
    print_solution(board);
    return;
  }
  if(board[i][j] != 0)
  {
    if(is_valid_up(board, i, j))
    {
      int aux = board[i-1][j];
      board[i-1][j] = board[i][j];
      board[i][j] = 0;
      for(std::size_t _i = 0; _i < board.size(); _i++)
        for(std::size_t _j = 0; _j < board[j].size(); _j++)
          find_solutions(board, count-1, _i, _j);
      board[i][j] = board[i-1][j];
      board[i-1][j] = aux;
    }
    if(is_valid_right(board, i, j))
    {
      int aux = board[i][j+1];
      board[i][j+1] = board[i][j];
      board[i][j] = 0;
      for(std::size_t _i = 0; _i < board.size(); _i++)
        for(std::size_t _j = 0; _j < board[j].size(); _j++)
          find_solutions(board, count-1, _i, _j);
      board[i][j] = board[i][j+1];
      board[i][j+1] = aux;
    }
    if(is_valid_down(board, i, j))
    {
      int aux = board[i+1][j];
      board[i+1][j] = board[i][j];
      board[i][j] = 0;
      for(std::size_t _i = 0; _i < board.size(); _i++)
        for(std::size_t _j = 0; _j < board[j].size(); _j++)
          find_solutions(board, count-1, _i, _j);
      board[i][j] = board[i+1][j];
      board[i+1][j] = aux;
    }
    if(is_valid_left(board, i, j))
    {
      int aux = board[i][j-1];
      board[i][j-1] = board[i][j];
      board[i][j] = 0;
      for(std::size_t _i = 0; _i < board.size(); _i++)
        for(std::size_t _j = 0; _j < board[j].size(); _j++)
          find_solutions(board, count-1, _i, _j);
      board[i][j] = board[i][j-1];
      board[i][j-1] = aux;
    }
  }
}

int main()
{
  int rows, columns, count = 0;
  std::cin >> rows >> columns;
  std::vector<std::vector<int>> board(rows, std::vector<int>(columns));

  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      std::cin >> board[i][j];
      if(board[i][j] > 0)
        count++;
    }
  }

  for(std::size_t i = 0; i < board.size(); i++)
  {
    for(std::size_t j = 0; j < board[i].size(); j++)
    {
      find_solutions(board, count, i, j);
    }
  }

  return 0;
}
