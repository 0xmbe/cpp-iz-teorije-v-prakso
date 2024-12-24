
#include "PegSolitaire.hpp"

const std::array<std::array<char, 9>, 9> ps::get_english_board() {
	std::fstream file("../PegSolitaireBoard.txt");
	std::array<std::array<char, 9>, 9> board = {};
	if (file.is_open()) {
		std::cout << "Reading file" << std::endl;
		std::string line;
		int row = 0;
		while (std::getline(file, line)) {
			auto s = line.size();
			for (int col = 0; col < line.size(); col++) {
				auto charValue = line[col];
				if (charValue == 'E') {
					return board;
				}
				else if (charValue == '1') {
					board[row][col] = '1';
				}
				else if (charValue == '0') {
					board[row][col] = '0';
				}
				else {
					board[row][col] = ' ';
				}
			}
			row++;
		}
	}
	else {
		std::cerr << "Error opening file" << std::endl;
	}
	return board;
}
void ps::print(const std::array<std::array<char, 9>, 9>* board) {
	for (auto& line : *board) {
		for (auto& chr : line) {
			std::cout << chr;
		}
		std::cout << std::endl;
	}
}
void updatePath(std::vector<std::array<char, 5>>* path, const char lastDirection,
	const char currentRow, const char currentCol, const char rowOffset, const char colOffset) {
	std::array<char, 5> temp = {};
	temp[0] = lastDirection;
	temp[1] = currentRow;
	temp[2] = currentCol;
	temp[3] = currentRow + rowOffset;
	temp[4] = currentCol + colOffset;
	path->push_back(temp);
}
static bool check_X_away(const std::array<std::array<char, 9>, 9>* board, const char currentRow, const char currentCol,
	const char rowOffset, const char colOffset, const char checkValue) {
	return ((*board)[currentRow + rowOffset][currentCol + colOffset] == checkValue);

}
static char remainingPieces(const std::array<std::array<char, 9>, 9>* board) {
	char remainingPieces = 0;
	for (auto& line : *board) {
		for (auto& chr : line) {
			if (chr == '1') {
				++remainingPieces;
				if (remainingPieces > 1) {
					return remainingPieces;			// Return even if you don't count them all
				}
			}
		}
	}
	return remainingPieces;
}
/// <summary>
/// FOR rowOffset AND colOffset USE ONLY VALUE -2, 0, +2 CORESPONDING TO AXIS DIRECTION
/// </summary>
static void makePegJump(std::array<std::array<char, 9>, 9>* board, const char currentRow, const char currentCol,
	const char rowOffset, const char colOffset, const char* setBits) {
	(*board)[currentRow][currentCol] = setBits[0];								  // PEACE MAKING A JUMP		//setBits_normal -> '0'
	(*board)[currentRow + rowOffset / 2][currentCol + colOffset / 2] = setBits[1];// PEACE THAT IS JUMPED OVER	//setBits_normal -> '0'
	(*board)[currentRow + rowOffset][currentCol + colOffset] = setBits[2];		  // PEACE NEW POSITION			//setBits_normal -> '1'
}
static void makeBoardSnapshot(std::vector<std::array<std::array<char, 9>, 9>>* boardSnapshots, const std::array<std::array<char, 9>, 9>* board) {
	boardSnapshots->push_back(*board);
}
static const char* getMoveName(move move) {
	switch (move) {
	case move::up:    return "up";
	case move::right: return "right";
	case move::down:  return "down";
	case move::left:  return "left";
	}
}
void ps::printPath(const std::vector<std::array<char, 5U>>* path) {
	std::cout << "PATH:\n";
	for (int i = 1; i < path->size(); ++i) {
		std::cout << "## " << i << ".) <" << getMoveName((move)(*path)[i][0]) << "> (" << (int)(*path)[i][1] << ", " << (int)(*path)[i][2] << ")->(" << (int)(*path)[i][3] << ", " << (int)(*path)[i][4] << ") ";
	}
	std::cout << "\n\n";
}
void printPathShort(std::vector<std::array<char, 5U>>* path) {
	for (auto& mov : *path) {
		std::cout << (int)mov[0];
	}
	std::cout << std::endl;
}
template<>
struct std::hash<std::tuple<std::array<std::array<char, 9>, 9>, char, char, char, char>> {
	std::uint32_t operator()(const tuple<array<array<char, 9>, 9>, const char, const char, const char, const char>& T) const noexcept {
		std::uint32_t hashVal = 0;
		std::hash<std::string> stringHasher;
		for (const auto& row : get<0>(T)) {
			hashVal ^= stringHasher(std::string(row.begin(), row.end())) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
		}
		hashVal ^= std::hash<char>{}(get<1>(T)) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
		hashVal ^= std::hash<char>{}(get<2>(T)) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
		hashVal ^= std::hash<char>{}(get<3>(T)) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
		hashVal ^= std::hash<char>{}(get<4>(T)) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
		return hashVal;
	}
};
void ps::playPathCommands(const std::vector<std::array<char, 5>>* path, std::array<std::array<char, 9>, 9> board) {
	const char setBits_normal[3] = { '0', '0', '1' };
	for (int i = 1; i < path->size(); ++i) {
		auto const direction = (*path)[i][0];
		auto const currentRow = (*path)[i][1];
		auto const currentCol = (*path)[i][2];
		auto const rowOffset = ((*path)[i][3] - (*path)[i][1]) / 2;
		auto const colOffset = ((*path)[i][4] - (*path)[i][2]) / 2;
		makePegJump(&board, currentRow, currentCol, (2 * rowOffset), (2 * colOffset), setBits_normal);	// JUMP
		std::cout << "## " << i << ".) <" << getMoveName((move)(*path)[i][0]) << "> (" << (int)(*path)[i][1] << ", " << (int)(*path)[i][2] << ")->(" << (int)(*path)[i][3] << ", " << (int)(*path)[i][4] << ") ";
		ps::print(&board);
		//_Thrd_sleep_for(500);
	}
}
void rotateBoard90DegreesClockwise(std::array<std::array<char, 9>, 9>* board) {
	std::array<std::array<char, 9>, 9> rotated = {};
	for (int i = 0; i < board->size(); ++i) {
		for (int j = 0; j < board->size(); ++j) {
			rotated[j][board->size() - 1 - i] = (*board)[i][j];
		}
	}
	*board = rotated;
}
std::vector<std::array<char, 5>> ps::solve(std::array<std::array<char, 9>, 9> board) {
	const std::array<std::array<char, 2>, 4> checkDirections = { {{-1, 0}, {0, 1}, {1, 0}, {0, -1}} };
	const char setBits_normal[3] = { '0', '0', '1' };
	char rowOffset = 0;
	char colOffset = 0;
	char direction = 0;
	char currentRow = 0;
	char currentCol = 0;
	bool moveMade = false;
	std::vector<std::array<char, 5>> path;
	std::vector<std::array<std::array<char, 9>, 9>> boardSnapshots;
	using KeyType = std::tuple<std::array<std::array<char, 9>, 9>, char, char, char, char>;
	KeyType currentBoardState_Key = {};
	std::uint32_t currentBoardState_Hash = {};
	std::unordered_set<std::uint32_t> boardHashSet;
	boardHashSet.reserve(2000000);//1999303
	makeBoardSnapshot(&boardSnapshots, &board);																				// ADD INITIAL BOARD TO SCANPSHOT
	updatePath(&path, 0, 0, 0, 0, 0);
	while (remainingPieces(&board) > 1) {																					// LOOP UNTIL > 1 PIECE ON BOARD
		moveMade = false;
		for (currentRow = 0; currentRow < board.size(); ++currentRow) {														// LOOP ROWS
			for (currentCol = 0; currentCol < board[currentRow].size(); ++currentCol) {										// LOOP COLUMNS
				if (board[currentRow][currentCol] == '1') {																	// IF PIECE IS MOVABLE
					for (direction = 0; direction < checkDirections.size(); ++direction) {									// LOOP 3 (4) POSSIBLE MOVE DIRECTIONS
						rowOffset = checkDirections[direction][0];															// SET OFFSET BASED ON DIRECTION
						colOffset = checkDirections[direction][1];
						if (check_X_away(&board, currentRow, currentCol, rowOffset, colOffset, '1')) {						// TEST 1st NEIGHBOUR BIT
							if (check_X_away(&board, currentRow, currentCol, (2 * rowOffset), (2 * colOffset), '0')) {		// TEST 2nd NEIGHBOUR BIT
								currentBoardState_Key = std::make_tuple(board, currentRow, currentCol, rowOffset, colOffset);		// GET HASH KEY
								currentBoardState_Hash = std::hash<KeyType>{}(currentBoardState_Key);								// CALCULATE HASH
								if (boardHashSet.find(currentBoardState_Hash) == boardHashSet.end()) {								// AND CHECK IF WE ALREADY TESTED IT 
									boardHashSet.insert(currentBoardState_Hash);													// STORE HASH
									makePegJump(&board, currentRow, currentCol, (2 * rowOffset), (2 * colOffset), setBits_normal);	// JUMP
									updatePath(&path, direction, currentRow, currentCol, (2 * rowOffset), (2 * colOffset));
									makeBoardSnapshot(&boardSnapshots, &board);
									currentRow = 0;
									currentCol = 0;
									direction = 0;
									moveMade = true;
									break;
								}
							}
						}
					}
				}
			}
		}
		if (!moveMade) {
			if (boardSnapshots.size() > 1) {
				//std::cout << "No moves made in this round. Backtracking to previous map state." << std::endl;
				boardSnapshots.pop_back();																						// REMOVE LAST BOARD ENTRY
				path.pop_back();																								// REMOVE LAST PATH ENTRY
				board = boardSnapshots.back();																					// REVERT PREVIOUS BOARD
				//rotateBoard90DegreesClockwise(&board);																		// ROTATE BOARD
			}
		}
	}
	//printPath(&path);
	//print(&board);
	//std::cout << "Tried positions: " << boardHashSet.size() << std::endl;
	return path;
}
