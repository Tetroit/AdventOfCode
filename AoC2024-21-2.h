#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"


struct Task {
	struct Pad {
		int width;
		int height;
		int posX = 0;
		int posY = 0;
		std::unordered_map<char, std::pair<int, int>> map;
		std::unordered_map<int, char> reverseMap;

		static Pad MakeNumpad() {
			Pad numpad(3, 4, 2, 3);
			numpad.AddControl('7', 0, 0);
			numpad.AddControl('8', 1, 0);
			numpad.AddControl('9', 2, 0);

			numpad.AddControl('4', 0, 1);
			numpad.AddControl('5', 1, 1);
			numpad.AddControl('6', 2, 1);

			numpad.AddControl('1', 0, 2);
			numpad.AddControl('2', 1, 2);
			numpad.AddControl('3', 2, 2);

			numpad.AddControl('0', 1, 3);
			numpad.AddControl('A', 2, 3);
			return numpad;
		}
		static Pad MakeDirpad() {
			Pad numpad(3, 2, 2, 0);
			numpad.AddControl('^', 1, 0);
			numpad.AddControl('A', 2, 0);

			numpad.AddControl('<', 0, 1);
			numpad.AddControl('v', 1, 1);
			numpad.AddControl('>', 2, 1);
			return numpad;
		}

		Pad(int width, int height, int posX = 0, int posY = 0) : width(width), height(height), posX(posX), posY(posY) {
			for (int i=0; i < width * height; i++) {
				reverseMap.try_emplace(i, 'X');
			}
		}
		void AddControl(char ch, int x, int y) {
			map.try_emplace(ch, x, y);
			reverseMap[width * y + x] = ch;
		}
		const std::pair<int, int>& GetCoords(char ch) const {
			return map.at(ch);
		}

		std::pair<int, int> GetPos() const {
			return {posX, posY};
		}
		void SetPos(int x, int y) {
			posX = x;
			posY = y;
		}

		static char GetDir (int dx, int dy) {
			if (dy == 0)
				return dx > 0 ? '>' : '<';
			if (dx == 0)
				return dy > 0 ? 'v' : '^';

			std::cerr << "Invalid direction " << dx << ", " << dy << '\n';
			return '?';
		}

		std::vector<std::string> GetPaths(char a, char b) const {
			std::vector<std::string> res;
			auto& [x1, y1] = GetCoords(a);
			auto& [x2, y2] = GetCoords(b);
			int dx = x2 - x1 > 0 ? 1 : -1;
			int dy = y2 - y1 > 0 ? 1 : -1;

			GeneratePaths(x1, y1, x2, y2, dx, dy, res, "");
			return res;
		}

		void GeneratePaths(int sx, int sy, int ex, int ey, int dx, int dy, std::vector<std::string>& arr, std::string path) const {
			if (reverseMap.at(sy * width + sx) == 'X')
				return;
			if (sx == ex && sy == ey) {
				arr.push_back("A" + path + 'A');
				return;
			}
			if (sx != ex) {
				GeneratePaths(sx + dx, sy, ex, ey, dx, dy, arr, path + GetDir(dx, 0));
			}
			if (sy != ey) {
				GeneratePaths(sx, sy + dy, ex, ey, dx, dy, arr, path + GetDir(0, dy));
			}
		}


		std::string Execute(const std::string& sequence, int x, int y) {
			posX = x;
			posY = y;
			std::string res;
			for (char ch : sequence) {
				if (ch == '<')
					posX--;
				else if (ch == '>')
					posX++;
				else if (ch == '^')
					posY--;
				else if (ch == 'v')
					posY++;
				else if (ch == 'A')
					res += reverseMap[width * posY + posX];

				if (reverseMap[posY * width + posX] == 'X') {
					std::cerr << res << " PANIC" << "\n";
				}
			}
			return res;
		}
	};


	std::unordered_map<int, long long> reg;

	void AddToReg(char a, char b, int depth, long long val) {
		int code = depth + (a << 8) + (b << 16);
		reg.try_emplace(code, val);
	}
	std::pair<bool, long long> GetFromReg(char a, char b, int depth) {
		int code = depth + (a << 8) + (b << 16);
		if (reg.find(code) != reg.end()) {
			return {true, reg[code]};
		}
		return {false, 0};
	}

	long long findFastest(const std::vector<std::string>& arr) {

		long long min = std::numeric_limits<long long>::max();
		for (auto& el : arr) {
			if (el.length() < min)
				min = el.length();
		}
		return min;
	}

	long long findFastestTransition (char a, char b, const Pad& pad, const Pad& dirPad, int depth) {
		auto paths = pad.GetPaths(a, b);
		if (depth == 0) {
			long long res = findFastest(paths) - 1;
			AddToReg(a, b, depth, res);
			return res;
		}
		long long min = std::numeric_limits<long long>::max();
		for (auto & path : paths) {
			long long total = 0;
			for (int j=1; j<path.size(); j++) {
				char a1 = path[j-1];
				char b1 = path[j];
				auto [success, cache] = GetFromReg(a1, b1, depth-1);
				if (success)
					total += cache;
				else {
					total += findFastestTransition(a1, b1, dirPad, dirPad, depth-1);
				}
			}
			if (total < min) {
				min = total;
			}
		}
		AddToReg(a, b, depth, min);
		return min;
	}

	void run() {

		std::ifstream inputStream("input2024-21.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::vector<std::string> codes;
		std::string line;
		while (std::getline(inputStream, line))
		{
			codes.push_back(line);
		}

		auto numpad = Pad::MakeNumpad();
		auto dirpad = Pad::MakeDirpad();

		long long score = 0;

		auto arr = numpad.GetPaths('A', '7');

		long long min = std::numeric_limits<long long>::max();

		long long testRes = findFastestTransition('A', '3', numpad, dirpad, 2);

		for (auto& code : codes) {

			auto extended = "A" + code;
			long long total = 0;
			for (int i = 1; i < extended.length(); i++) {
				total += findFastestTransition(extended[i-1], extended[i], numpad, dirpad, 25);
			}

			long long num = std::stoll(code.substr(0, 3));
			score += total * num;
			std::cout << total << " * " << num << "\n";
		}

		std::cout << score << '\n';
		//211204
		//212194
		//213536
		//213738
		//218300
		//220984

		//294667840961390
		//253834885391182
		//255934827640042
		//258369757013802
		//91461302914403

		//OH GOD THAT TOOK ME 2 DAYS
	}
};
