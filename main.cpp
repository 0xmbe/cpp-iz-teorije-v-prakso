#include "PegSolitaire.hpp"

#include <iostream> 
#include <chrono>

int main()
{
  using namespace std::chrono;
  auto const english_board = ps::get_english_board();

  std::cout << "Start solving:\n";
  ps::print(english_board);
  std::cout << "---------------\n";

  auto const start = high_resolution_clock::now();

  // Solving board e.g.
  // 
  // Add your code
  // 
  // ps::solve(english_board, moves); or
  // const auto moves = ps::solve(english_board);

  auto end = high_resolution_clock::now();
  auto duration_ms = duration_cast<duration<double, std::milli>>(end - start).count();

  // Print moves
  //
  // Add your code
  // e.g.
  // ps::print_moves(results);
  std::cout << "Solving took: " << duration_ms << " ms\n";

  /*
  Output example:
  ~~~~~~~~~~~~~~~~
  Start solving:
  111
  111
1111111
1110111
1111111
  111
  111
---------------
  111
  111
1111111
1110111
1111111
  111
  111

  111
  111
1111111
1111001
1111111
  111
  111

[ ... omitted ...]

  000
  000
0000000
0000000
0001000
  010
  000

  000
  000
0000000
0000000
0000000
  000
  010

Solving took: 492.057 ms
  */

  return 0;
}
