#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

//intcode robot driver

enum struct HaltCode {
	NONE = 0,
	EXIT = 1,
	WAIT = 2,
	ERROR = 3,
};

template <typename T>
struct Intcode {
	std::vector<T> program;
	std::vector<T> reserve;
	int ptr=0;
	int base=0;

	void start() {
		reserve = program;
	}
	void end() {
		program = reserve;
		ptr = 0;
		base = 0;
	}
	static void decypher(int instruction, int& opcode, int& mode1, int& mode2, int& mode3) {
		opcode = instruction % 100;
		instruction /= 100;
		mode1 = instruction % 10;
		instruction /= 10;
		mode2 = instruction % 10;
		instruction /= 10;
		mode3 = instruction % 10;
	}
	T& get (T pos) {
		if (pos >= program.size()) {
			program.resize(pos + 1);
		}
		return program[pos];
	}
	T getByMode(T pos, int mode) {
		if (mode == 1) return pos;
		if (mode == 2) return get(base+pos);
		return get(pos);
	}
	T& getRefByMode(T pos, int mode) {
		if (mode == 1) std::cerr << "Cannot write to rvalue" << std::endl;
		if (mode == 2) return get(base+pos);
		return get(pos);
	}
	bool executeUntilInput(std::deque<T>& input, std::deque<T>& output, HaltCode& haltCode) {
		if (get(ptr) % 100 == 3 && input.empty()) {
			haltCode = HaltCode::WAIT;
			return false;
		}
		return execute(input, output, haltCode);
	}
	bool execute(std::deque<T>& input, std::deque<T>& output, HaltCode& haltCode) {
		int mode1 = 0;
		int mode2 = 0;
		int mode3 = 0;
		int code = 0;
		decypher(get(ptr), code, mode1, mode2, mode3);
		if (code == 99) {
			haltCode = HaltCode::EXIT;
			return false;
		}
		switch (code) {
			case 1: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) + getByMode(get(ptr+2), mode2);
				ptr+=4;
				break;
			}
			case 2: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) * getByMode(get(ptr+2), mode2);
				ptr+=4;
				break;
			}
			case 3: {
				getRefByMode(get(ptr+1), mode1) = input.front();
				input.pop_front();
				ptr+=2;
				break;
			}
			case 4: {
				output.push_back(getByMode(get(ptr+1), mode1));
				ptr+=2;
				break;
			}
			case 5: {
				ptr = getByMode(get(ptr+1), mode1) != 0 ? getByMode(get(ptr+2), mode2) : ptr+3;
				break;
			}
			case 6: {
				ptr = getByMode(get(ptr+1), mode1) == 0 ? getByMode(get(ptr+2), mode2) : ptr+3;
				break;
			}
			case 7: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) < getByMode(get(ptr+2), mode2) ? 1 : 0;
				ptr+=4;
				break;
			}
			case 8: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) == getByMode(get(ptr+2), mode2) ? 1 : 0;
				ptr+=4;
				break;
			}
			case 9: {
				base += getByMode(get(ptr+1), mode1);
				ptr+=2;
				break;
			}
			default: {
				std::cout << "Unknown code: " << code << std::endl;
				haltCode = HaltCode::ERROR;
				return false;
			}
		}
		return true;
	}
};

struct Task
{

	static inline const std::vector<ivec2> dirMap{{1,0},{0, -1},{-1, 0},{0,1}};

	static inline std::unordered_map<ivec2, int, ivec2hash> field;
	static inline Intcode<long long> computer;

	static void run() {
		std::ifstream inputStream("2019/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		long long num;
		while (inputStream >> num)
		{
			inputStream.ignore(1);
			computer.program.push_back(num);
		}
		inputStream.close();
		std::deque<long long> input{0};
		std::deque<long long> output;
		HaltCode haltCode = HaltCode::NONE;

		ivec2 pos = {0,0};
		int dir = 1;
		computer.start();
		while (true) {
			while (computer.executeUntilInput(input, output, haltCode));
			if (haltCode == HaltCode::EXIT) break;
			int col = output.front();
			output.pop_front();
			int turn = output.front();
			output.pop_front();

			field[pos] = col;
			if (turn == 0) dir = (dir + 1) % 4;
			else if (turn == 1) dir = (dir + 3) % 4;
			pos += dirMap[dir];

			input.push_back(field[pos]);
		}
		computer.end();
		std::cout << field.size() << std::endl;
	}
	static void runPart2() {
		field.clear();
		std::deque<long long> input{1};
		std::deque<long long> output;
		HaltCode haltCode = HaltCode::NONE;

		ivec2 pos = {0,0};
		int dir = 1;
		computer.start();
		while (true) {
			while (computer.executeUntilInput(input, output, haltCode));
			if (haltCode == HaltCode::EXIT) break;
			int col = output.front();
			output.pop_front();
			int turn = output.front();
			output.pop_front();

			field[pos] = col;
			if (turn == 0) dir = (dir + 1) % 4;
			else if (turn == 1) dir = (dir + 3) % 4;
			pos += dirMap[dir];

			input.push_back(field[pos]);
		}
		computer.end();

		int minX = INT_MAX;
		int minY = INT_MAX;
		int maxX = INT_MIN;
		int maxY = INT_MIN;
		for (auto& [key, val] : field) {
			if (key.x < minX) minX = key.x;
			if (key.x > maxX) maxX = key.x;
			if (key.y < minY) minY = key.y;
			if (key.y > maxY) maxY = key.y;
		}
		DynamicGrid<int> display;
		display.resize(maxX - minX + 1, maxY - minY + 1);
		for (auto& [key, val] : field) {
			display.set(key.x - minX, key.y - minY, val);
		}
		display.print([](int val ){return val == 1 ? '#' : '.';});
	}
};

//-------------- NOTES AREA ----------------
/*

*/