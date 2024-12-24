#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <regex>


enum move {
	up,
	right,
	down,
	left
};
class ps {
public:
	const static std::array<std::array<char, 9>, 9> get_english_board();
	static void print(const std::array<std::array<char, 9>, 9>* board);
	static void playPathCommands(const std::vector<std::array<char, 5>>* path, std::array<std::array<char, 9>, 9> board);
	static void printPath(const std::vector<std::array<char, 5U>>* path);
	static std::vector<std::array<char, 5>> solve(std::array<std::array<char, 9>, 9> board);
};
