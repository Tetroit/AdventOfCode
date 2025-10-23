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

const std::unordered_map<std::string, std::vector<std::string>> patternLookup
{
	{"A", {"A"}},
	{"^A", {"<A", ">A"}},
	{"<A", {"v<<A", ">>^A"}},
	{">A", {"vA", "^A"}},
	{"vA", {"<vA", "^>A"}},
	{"<^A", {"v<<A","^>A",">A"}},
	{"^<A", {"<A","v<A",">>^A"}},
	{"<vA", {"v<<A",">A","^>A"}},
	{"v<A", {"<vA","<A",">>^A"}},
	{">^A", {"vA","<^A",">A"}},
	{"^>A", {"<A","v>A","^A"}},
	{"v>A", {"<vA",">A","^A"}},
	{">vA", {"v<<A",">A","^>A"}},
	{"v<<A", {"<vA","<A","A",">>^A"}},
	{">>^A", {"vA","A","<^A",">A"}},
};

const std::unordered_map<std::string, std::vector<std::string>> patternLookupUpPriority
{
	{"A", {"A"}},
	{"^A", {"<A", ">A"}},
	{"<A", {"v<<A", ">>^A"}},
	{">A", {"vA", "^A"}},
	{"vA", {"<vA", "^>A"}},
	{"<^A", {"v<<A","^>A",">A"}},
	{"^<A", {"<A","v<A",">>^A"}},
	{"<vA", {"v<<A",">A","^>A"}},
	{"v<A", {"<vA","<A",">>^A"}},
	{">^A", {"vA","<^A",">A"}},
	{"^>A", {"<A","v>A","^A"}},
	{"v>A", {"<vA",">A","^A"}},
	{">vA", {"v<<A",">A","^>A"}},
	{"v<<A", {"<vA","<A","A",">>^A"}},
	{">>^A", {"vA","A","<^A",">A"}},
};

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

		std::pair<int, int> GetPos() {
			return {posX, posY};
		}
		void SetPos(int x, int y) {
			posX = x;
			posY = y;
		}

		std::string MoveY(int y1, int y2) {
			std::string res;
			if (y1 < y2) {
				for (int y = y1; y < y2; y++) {
					res += 'v';
				}
			}
			else {
				for (int y = y1; y > y2; y--) {
					res += '^';
				}
			}
			return res;
		}
		std::string MoveX(int x1, int x2) {
			std::string res;
			if (x1 < x2) {
				for (int x = x1; x < x2; x++) {
					res += '>';
				}
			}
			else {
				for (int x = x1; x > x2; x--) {
					res += '<';
				}
			}
			return res;
		}

		std::string GetInstructions(const char& end, bool upFirst = false) {
			std::string res;
			int x1 = posX;
			int y1 = posY;
			auto& [x2, y2] = GetCoords(end);

			char prevMove = 0;

			bool yPriority = false;
			if (x2 < x1) {

				if (reverseMap[y1 * width + x2] == 'X')
					yPriority = true;
				else
					yPriority = false;
			}
			else if (y2 > y1) {

				if (reverseMap[y2 * width + x1] == 'X')
					yPriority = false;
				else
					yPriority = true;
			}

			else if (upFirst && x1 < x2 && y1 > y2){

				if (reverseMap[y2 * width + x1] == 'X')
					yPriority = false;
				else
					yPriority = true;
			}

			if (yPriority) {
				res += MoveY(y1, y2);
				res += MoveX(x1, x2);
			}
			else {
				res += MoveX(x1, x2);
				res += MoveY(y1, y2);
			}

			posX = x2;
			posY = y2;
			res += 'A';
			return res;
		}
		std::string GetInstructions(const std::string& sequence, bool upFirst = false) {
			std::string res;
			for (const char& ch : sequence) {
				res += GetInstructions(ch, upFirst);
			}
			return res;
		}

		std::vector<std::string> GetAllInstructions(const char& end) {

			std::vector<std::string> allRes;
			std::string res;

			int x1 = posX;
			int y1 = posY;
			auto& [x2, y2] = GetCoords(end);

			bool canStartWithX = reverseMap[y1 * width + x2] != 'X';
			bool canStartWithY = reverseMap[y2 * width + x1] != 'X';

			if (canStartWithY) {
				res += MoveY(y1, y2);
				res += MoveX(x1, x2);
				res += 'A';
				allRes.push_back(res);
				res.clear();
			}
			if (canStartWithX && x1 != x2 && y1 != y2) {
				res += MoveX(x1, x2);
				res += MoveY(y1, y2);
				res += 'A';
				allRes.push_back(res);
				res.clear();
			}

			posX = x2;
			posY = y2;

			return allRes;
		}

		std::vector<std::string> GetAllInstructions(const std::string& sequence) {

			std::vector<std::vector<std::string>> fragments;
			int size = 1;
			std::vector<std::string> allRes;

			for (const char& ch : sequence) {
				std::vector<std::string> arr = GetAllInstructions(ch);
				size *= arr.size();
				fragments.push_back(arr);
			}
			for (int i =0; i < size; i++) {
				auto& res = allRes.emplace_back();
				int segments = 1;
				for (int j = 0; j < fragments.size(); j++) {
					int options = fragments[j].size();
					segments *= options;
					int optionID = (int)(((long long)i * segments) / size) % options;
					allRes[i] += fragments[j][optionID];
				}
			}
			return allRes;
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


	std::unordered_map<std::string, long long> GetPatternMap (std::string str)
	{
		std::unordered_map<std::string, long long> res;

		int id = 0;
		while (true) {
			int sep = str.find('A', id);
			if (sep == std::string::npos)
				return res;
			auto pattern = str.substr(id, sep-id+1);
			res[pattern]++;

			id = sep + 1;

		}
	}

	void Expand (std::unordered_map<std::string, long long>& map, const std::unordered_map<std::string, std::vector<std::string>>& lookup = patternLookup)
	{
		std::unordered_map<std::string, long long> toAdd;
		for (auto& [key, val] : map) {
			auto loc = lookup.find(key);
			if (loc != lookup.end()) {
				for (auto& component : loc->second) {
					toAdd[component] += val;
				}
			}
			else {
				std::cerr << key << " was not found \n";
			}
		}
		map.clear();
		for (auto& [key, val] : toAdd) {
			map[key] = val;
		}
	}

	void ExpandDyn (std::unordered_map<std::string, long long>& map, std::unordered_map<std::string, std::vector<std::string>>& lookup)
	{
		std::unordered_map<std::string, long long> toAdd;
		for (auto& [key, val] : map) {
			auto loc = lookup.find(key);
			if (loc != lookup.end()) {
				for (auto& component : loc->second) {
					toAdd[component] += val;
				}
			}
			else {
				TestAddPattern(lookup, key);
				auto loc = lookup.find(key);
				for (auto& component : loc->second) {
					toAdd[component] += val;
				}
			}
		}
		map.clear();
		for (auto& [key, val] : toAdd) {
			map[key] = val;
		}
	}

	long long Count(const std::unordered_map<std::string, long long>& map) {
		long long cnt = 0;
		for (const auto& [key, count] : map) {
			cnt += (long long) key.length() * count;
		}
		return cnt;
	}

	int GetMin (const std::vector<std::string>& vec) {
		int min = std::numeric_limits<int>::max();
		int minID = 0;
		for (int i=0; i<vec.size(); i++) {
			if (min < vec[i].length()) {
				min = vec[i].length();
				minID = i;
			}
		}
		return minID;
	}

	void SetTranslation (std::unordered_map<std::string, std::vector<std::string>>& lookup, const std::string& pattern, const std::string& translation) {

		if (lookup.find(pattern) == lookup.end()) {
			lookup.emplace(pattern, std::vector<std::string>{});
		}
		else {
			lookup[pattern].clear();
		}

		int id = 0;
		while (true) {
			int sep = translation.find('A', id);
			if (sep == std::string::npos)
				break;
			auto comm = translation.substr(id, sep-id+1);
			lookup[pattern].push_back(comm);
			id = sep + 1;
		}
	}
	void TestAddPattern(std::unordered_map<std::string, std::vector<std::string>>& lookup, const std::string& pattern) {

		auto pad = Pad::MakeDirpad();

		auto allCombos = pad.GetAllInstructions(pattern);
		std::string best;
		int min = std::numeric_limits<int>::max();

		//std::cout << pattern << ":\n";
		for (auto& combo : allCombos) {

			auto map = GetPatternMap(combo);
			//std::cout << combo << ":\n";

			SetTranslation(lookup, pattern, combo);

			for (int i=0; i<10; i++) {
				ExpandDyn(map, lookup);
			}

			int cnt = Count(map);

			if (cnt < min) {
				min = cnt;
				best = combo;
			}
			//std::cout << Count(map) << "\n";
		}

		SetTranslation(lookup, pattern, best);
		//std::cout << "-------------------\n";


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
		std::string instruction;

		std::unordered_map<std::string, std::vector<std::string>> testLookup
		{
			{"A", {"A"}},
			{"^A", {"<A", ">A"}},
			{"<A", {"v<<A", ">>^A"}},
			{">A", {"vA", "^A"}},
			{"vA", {"<vA", "^>A"}},
			{"<^A", {"v<<A","^>A",">A"}},
			{"^<A", {"<A","v<A",">>^A"}},
			{"<vA", {"v<<A",">A","^>A"}},
			{"v<A", {"<vA","<A",">>^A"}},
			{">^A", {"vA","<^A",">A"}},
			{"^>A", {"<A","v>A","^A"}},
			{"v>A", {"<vA",">A","^A"}},
			{">vA", {"v<<A",">A","^>A"}},
			{"v<<A", {"<vA","<A","A",">>^A"}},
			{">>^A", {"vA","A","<^A",">A"}},
		};
		for (const auto& [key, val] : testLookup) {

			//std::cout << key << ":\n";

			std::string testinst = key;

			auto allCombos = dirpad.GetAllInstructions(testinst);

			for (auto& combo : allCombos) {


				auto map = GetPatternMap(combo);
				// std::cout << combo << ":\n";

				for (int i=0; i<10; i++) {
					ExpandDyn(map, testLookup);
				}
				// std::cout << Count(map) << "\n";
			}
			// std::cout << "-------------------\n";
		}

		testLookup = patternLookup;

		for (auto& code : codes) {

			auto collection = numpad.GetAllInstructions(code);

			long long cnt;

			long long min = std::numeric_limits<long long>::max();

			for (auto& item : collection) {

				auto patternsUp = GetPatternMap(item);

				for (int i=0; i<25; i++) {
					ExpandDyn(patternsUp, testLookup);
				}
				cnt = Count(patternsUp);
				std::cout << cnt << "\n";
				if (cnt < min)
					min = cnt;
			}

			long long num = std::stoll(code.substr(0, 3));

			score += min * num;

			std::cout << min << " * " << num << "\n";

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
		//91461302914403

		// 029A: <vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A

			  // <<vA>>^AAAvA^A<<vAA>A>^AvAA<^A>A<<vA>A>^AAAvA<^A>A<vA>^A<A>A
		// 980A: <v<A>>^AAAvA^A<vA<AA>>^AvAA<^A>A<v<A>A>^AAAvA<^A>A<vA>^A<A>A

			  // v<<A>>^A<vA<A>>^AAvA<^A>AvA^A<vA>^A<A>A<vA>^A<A>Av<<A>A>^AAvA<^A>A
		// 179A: <v<A>>^A<vA<A>>^AAvAA<^A>A<v<A>>^AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A

			  // <<vAA>A>^AAvA<^A>AAvA^A<vA>^A<A>A<vA>^A<A>A<<vA>A>^AAvA<^A>A
		// 456A: <v<A>>^AA<vA<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^A<A>A<v<A>A>^AAvA<^A>A

		// v<<A>>^AvA^Av<<A>>^AAv<A<A>>^AAvAA^<A>Av<A^>AA<A>Av<A<A>>^AAA<Av>A^A
		// <<vA^>>AvA^A<<vAA>A>^AA<A>vA^AAvA^A<vA>^AA<A>A<<vA>A>^AAAvA<^A>A

		//        33021301021 = 16
		// <A>A | <AAv<AA>>^A | vAA^Av<AAA^>A
		//        30120121011 = 12
		// <A>A | <<vAA^>AA>A | vAA^A<vAAA>^A

		// ^A | ^^<<A | >>AvvvA
		// ^A | <<^^A | >>AvvvA

		// 379A: <v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A

		//<<vA -> <<vAA>A>^A	| 3 moves -> 0 moves -> 1 moves BACK -> 2 moves BACK
		//v<<A -> <vA<AA>>^A	| 2 moves -> 1 moves -> 0 moves -> 3 moves BACK
		//<v<A -> <<vA>A<A>>^A	| 3 moves -> 1 move L -> 1 move L -> 3 moves BACK ======= BAD

		// v<<A>>^A<vA<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^Av<<A>^A>AAvA^Av<<A>A>^AAAvA<^A>A
		// <Av<AA>>^AvA^AvA<^AA>A<vAAA>^A
		// ^<<A>A>^^AvvvA
		// 129A

		// v<<A>>^AAAvA^A<vA<AA>>^AAvAA<^A>Av<<A>A>^AvA<^A>A<vA>^AAv<<A>>^AAvA<^A>A
		// <AAA>Av<<AA>>^A<vA>^AvAA<AA>^A
		// ^^^A<<AvA>>vvA
		// 974A

		// <vA<AA>>^AvA<^A>AAAvA^Av<<A>A>^AAAvA<^A>Av<<A>>^AAvA^A<vA>^Av<<A>>^AAvA<^A>A
		// v<<A>^AAA>A<vAAA>^A<AA>AvA<AA>^A
		// <^^^AvvvA^^A>vvA
		// 805A

		// v<<A>>^AAvA^A<vA<AA>>^AAvA<^A>AvA^Av<<A>A>^AAvA<^A>A<vA>^AAv<<A>>^AvA<^A>A
		// <AA>Av<<AA>^A>A<vAA>^AvAA<A>^A
		// ^^A<<^AvvA>>vA
		// 671A

		// v<<A>>^AvA^A<vA<AA>>^AvA<^A>AAvA^A<vA>^Av<<A>>^AvA<^A>Av<<A>A>^AAvA<^A>A
		// <A>Av<<A>^AA>AvA<A>^A<vAA>^A
		// ^A<^^A>vAvvA
		// 386A

		//WHY THE HELL DOES ONE PERFORM BETTER THAN THE OTHER??!??

		//
		// v<<A>>^AvA^A<vA<AA>>^AvA<^A>AAvA^A<vA>^Av<<A>>^AvA<^A>Av<<A>A>^AAvA<^A>A
		// v<<A>>^AvA^A<vA<AA>>^AvA<^A>AAvA^A<vA<A>>^AvA^A<A>A<vA<A>>^AA<A>vA^A
		// <A>Av<<A>^AA>AvA<A>^A<vAA>^A
		// <A>Av<<A>^AA>Av<A>A^Av<AA^>A
		// ^A<^^A>vAvvA
		// ^A<^^Av>AvvA

		//its the f#*ing down button being 2 steps away, so I have to put priorities

		//<A>Av<<A>^AA>A<vA>A^A<vAA>^A
		//v<<A>>^AvA^A<vA<AA>>^AvA<^A>AAvA^Av<<A>A>^AvA^A<A>Av<<A>A>^AAvA<^A>A
		//<vA<AA>>^AvAA<^A>A<vA>^A<A>Av<<A>A>^Av<<A>>^AAvAA<^A>A<vA>^Av<<A>^A>AvA^AA<vA>^A<A>A<vA<AA>>^AvA^AvA<^A>A<vA>^A<A>Av<<A>>^AvA^A<vA<AA>>^AvA^AvA<^A>AA<vA>^Av<<A>^A>AvA^A
		//v<<A>A>^Av<<A>>^AAvAA<^A>A<vA>^AAv<<A>^A>AvA^Av<<A>A>^AvA<^A>Av<<A>>^AvA^A<vA<AA>>^AvA^AvA<^A>A<vA<AA>>^AvAA<^A>AA<vA>^AAv<<A>^A>AvA^Av<<A>A>^AvA<^A>A<vA<AA>>^AvA<^A>AvA^A<vA>^A<A>AAv<<A>A>^AvA<^A>Av<<A>>^AvA^Av<<A>A>^Av<<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^Av<<A>^A>AvA^Av<<A>A>^AvA<^A>Av<<A>>^AvA^A<vA<AA>>^AvAA<^A>A<vA>^A<A>Av<<A>A>^Av<<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^Av<<A>^A>AvA^AAv<<A>A>^AvA<^A>A<vA<AA>>^AvA<^A>AvA^A<vA>^A<A>A
		//
		// each A combination can be made in a dictionary
		// with corresponding count of next gen instructions and also split next gen into A combinations

		// A,
		// <A, >A, ^A, vA,
		// >^A, <vA, <^A, >vA
		// OHH GOOOHD THERES DIFFERENCE THAT IS VISIBLE ONLY AFTER 5 STEPS T_T

		//this is useless... lets start anew
	}
};
