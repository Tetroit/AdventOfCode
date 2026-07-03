#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

//scrambling password with commands

struct LetterMap {
	std::unordered_map<char, int> letterToID;
	std::unordered_map<int, char> IDToLetter;
	LetterMap(std::string pass) {
		for (int i=0; i<pass.length(); i++) {
			letterToID[pass[i]] = i;
			IDToLetter[i] = pass[i];
		}
	}
	void Set(char ch, int i) {
		letterToID[ch] = i;
		IDToLetter[i] = ch;
	}
	void SyncFromLTID() {
		for (const auto& [ch, id] : letterToID) {
			IDToLetter[id] = ch;
		}
	}
	void SyncFromIDTL() {
		for (const auto& [id, ch] : IDToLetter) {
			letterToID[ch] = id;
		}
	}
};

struct Task
{
	static inline std::vector<std::function<void(LetterMap&)>> program;
	static void run() {

		std::ifstream inputStream("2016/21.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::string word;
		while (inputStream >> word)
		{
			if (word == "swap") {
				inputStream >> word;
				if (word == "letter") {
					char a, b;
					inputStream >> a >> word >> word >> b;
					program.emplace_back([a, b](LetterMap& letterMap) {
						int x = letterMap.letterToID[a];
						int y = letterMap.letterToID[b];
						letterMap.Set(a, y);
						letterMap.Set(b, x);
					});
				}
				else if (word == "position") {
					int x, y;
					inputStream >> x >> word >> word >> y;
					program.emplace_back([x, y](LetterMap& letterMap) {
						char a = letterMap.IDToLetter[x];
						char b = letterMap.IDToLetter[y];
						letterMap.Set(a, y);
						letterMap.Set(b, x);
					});
				}
			}
			else if (word == "rotate") {
				int amount;
				inputStream >> word;
				if (word == "based") {
					char l;
					inputStream >> word >> word >> word >> word >> l;
					program.emplace_back([l](LetterMap& letterMap) {
						int amount = letterMap.letterToID[l];
						amount += amount >= 4 ? 2 : 1;
						int len = letterMap.letterToID.size();
						for (auto& [ch, id] : letterMap.letterToID) {
							id = Utils::FloorMod(id + amount, len);
						}
						letterMap.SyncFromLTID();
					});
				}
				else {
					bool reverse = word == "left";
					inputStream >> amount >> word;
					program.emplace_back([reverse, amount](LetterMap& letterMap) {
						int len = letterMap.letterToID.size();
						for (auto& [ch, id] : letterMap.letterToID) {
							if (reverse)
								id = Utils::FloorMod(id - amount, len);
							else
								id = Utils::FloorMod(id + amount, len);
						}
						letterMap.SyncFromLTID();
					});
				}
			}
			else if (word == "reverse") {
				int start, end;
				inputStream >> word >> start >> word >> end;
				program.emplace_back([start, end](LetterMap& letterMap) {
					int span = end - start + 1;
					for (int i=0; i< (span >> 1); i++) {
						int x = start + i;
						int y = end - i;
						char a = letterMap.IDToLetter[x];
						char b = letterMap.IDToLetter[y];
						letterMap.Set(a, y);
						letterMap.Set(b, x);
					}
				});
			}
			else if (word == "move") {
				int from, to;
				inputStream >> word >> from >> word >> word >> to;
				if (to > from) {
					program.emplace_back([from, to](LetterMap& letterMap) {
						char ch = letterMap.IDToLetter[from];
						for (int i=from; i<to; i++) {
							char current = letterMap.IDToLetter[i+1];
							letterMap.Set(current, i);
						}
						letterMap.Set(ch, to);
					});
				}
				else if (to < from) {
					program.emplace_back([from, to](LetterMap& letterMap) {
						char ch = letterMap.IDToLetter[from];
						for (int i=from; i>to; i--) {
							char current = letterMap.IDToLetter[i-1];
							letterMap.Set(current, i);
						}
						letterMap.Set(ch, to);
					});
				}
			}
		}
		inputStream.close();
		LetterMap map("abcdefgh");
		for (const auto& command : program) {
			command(map);
			for (int i=0; i< map.IDToLetter.size(); i++) {
				std::cout << map.IDToLetter[i];
			}
			std::cout << std::endl;
		}
	}
	static void runPart2() {
		std::string init = "abcdefgh";
		std::string scrambled = "fbgdceah";
		std::unordered_map<int, int> unscramble;
		do {
			LetterMap map(init);
			for (const auto& command : program) {
				command(map);
			}
			std::string res;
			for (int i=0; i< map.IDToLetter.size(); i++) {
				res += map.IDToLetter[i];
			}
			if (res == scrambled) {
				std::cout << init << std::endl;
			}
		} while (std::next_permutation(init.begin(), init.end()));
	}
};

//-------------- NOTES AREA ----------------
/*

*/