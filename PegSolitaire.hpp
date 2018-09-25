#include "PegSolitaire.hpp"
#include <iostream>

int main()
{
	auto const english_board = get_english_board();	// english_board->print(); ??

	print(english_board);
	std::cout << "Peg count: " << english_board.peg_count() << "\n";

	return 0;
}
