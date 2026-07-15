#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//2x2 -> 3x3 -> 4x4 -> split pattern recognition

struct Pattern {
	explicit Pattern(int size) : size(size) {}
	Pattern (int n, std::vector<Pattern>& patterns) : size(n * patterns[0].size) {
		int fragSize = patterns[0].size;
		for (int b = 0; b < n*n; b++) {
			int bx = b % n * fragSize;
			int by = b / n * fragSize;
			for (int x = 0; x < fragSize; x++) {
				for (int y = 0; y < fragSize; y++) {
					setBit(bx + x, by + y, patterns[b].getBit(x,y));
				}
			}
		}
	}
	using dataT = std::bitset<81>;
	dataT data = 0;
	int size;
	[[nodiscard]] bool getBit(int x, int y) const {
		int id = x + y * size;
		return data[id];
	}
	void setBit (int x, int y, bool bit) {
		int id = x + y * size;
		data[id] = bit;
	}
	[[nodiscard]] Pattern rotate() const {
		Pattern p(size);
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				p.setBit(size - 1 - y, x, getBit(x, y));
			}
		}
		return p;
	}
	[[nodiscard]] Pattern flipX() const {
		Pattern p(size);
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				p.setBit(size - 1 - x, y, getBit(x, y));
			}
		}
		return p;
	}
	[[nodiscard]] std::vector<Pattern> split(int n = 2) const {
		int newSize = size/n;
		std::vector<Pattern> res(n*n, Pattern(newSize));
		for (int b = 0; b < n*n; b++) {
			int bx = b % n * newSize;
			int by = b / n * newSize;
			for (int x = 0; x < newSize; x++) {
				for (int y = 0; y < newSize; y++) {
					res[b].setBit(x, y, getBit(bx + x, by + y));
				}
			}
		}
		return res;
	}
	[[nodiscard]] int count() const {
		int cnt = 0;
		for (int i=0; i<size*size; i++) {
			if (data[i]) cnt++;
		}
		return cnt;
	}
	void print() {
		for (int x=0; x<size; x++) {
			for (int y=0; y<size; y++) {
				std::cout << (data[x + y*size] ? '#' : '.');
			}
			std::cout << std::endl;
		}
	}
	std::string toString() {
		std::string res;
		for (int x=0; x<size; x++) {
			for (int y=0; y<size; y++) {
				res+= (data[x + y*size] ? '#' : '.');
			}
			res += '\n';
		}
		return res;
	}
	template <typename Fn>
	requires std::invocable<Fn&, const Pattern&>
	void foreachVariation(Fn&& fn) {
		Pattern p = *this;
		fn(*this);
		for (int i=0; i<3; i++) {
			p = p.rotate();
			fn(p);
		}
		p = p.flipX();
		fn(p);
		for (int i=0; i<3; i++) {
			p = p.rotate();
			fn(p);
		}
	}

	bool operator==(const Pattern & other) const {
		return size == other.size && data == other.data;
	}
};

struct PatternHasher {
	size_t operator()(const Pattern& obj) const noexcept {
		return std::hash<int>()(obj.size) ^ std::hash<std::string>()(obj.data.to_string());
	}
};

struct Task
{
	static inline std::unordered_map<Pattern, Pattern, PatternHasher> match2to3;
	static inline std::unordered_map<Pattern, Pattern, PatternHasher> match3to4;
	static Pattern match(Pattern p) {
		if (p.size == 3) {
			if (!match3to4.contains(p)) {
				std::cerr << "Couldnt find\n" << p.toString() << "in match3to4";
			}
			return match3to4.at(p);
		}
		if (p.size == 2) {
			if (!match2to3.contains(p)) {
				std::cerr << "Couldnt find\n" << p.toString() << "in match2to3";
			}
			return match2to3.at(p);
		}
	}
	static std::vector<Pattern> expand3(Pattern p) {
		Pattern p6 = expand(p, 2);
		std::vector<Pattern> frags6 = p6.split(3);
		std::vector<Pattern> frags6expanded(frags6.size(), Pattern(3));
		for (int i=0; i<frags6.size(); i++) {
			frags6expanded[i] = match(frags6[i]);
		}
		return frags6expanded;
	}
	static Pattern expand(Pattern p, int n = 2) {
		if (p.size != 3) {
			std::cout << "Wrong dimension" << std::endl;
			return Pattern(0);
		}
		Pattern p4 = match(p);
		if (n == 1) return p4;
		std::vector<Pattern> frags4 = p4.split(2);
		std::vector<Pattern> frags4expanded(frags4.size(), Pattern(3));
		for (int i=0; i<frags4.size(); i++) {
			frags4expanded[i] = match(frags4[i]);
		}
		Pattern p6 (2, frags4expanded);
		return p6;
	}
	static void run() {

		std::ifstream inputStream("2017/21.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string key, val, arrow;
		while (inputStream >> key >> arrow >> val)
		{
			std::vector<std::string> keyRows = Utils::split(key, '/');
			std::vector<std::string> valRows = Utils::split(val, '/');
			Pattern keyP(keyRows.size());
			for (int x=0; x<keyRows.size(); x++) {
				for (int y=0; y<keyRows.size(); y++) {
					keyP.setBit(x, y, keyRows[y][x] == '#' ? 1 : 0);
				}
			}
			Pattern valP(valRows.size());
			for (int x=0; x<valRows.size(); x++) {
				for (int y=0; y<valRows.size(); y++) {
					valP.setBit(x, y, valRows[y][x] == '#' ? 1 : 0);
				}
			}
			if (keyP.size == 2) {
				keyP.foreachVariation([&valP](const Pattern& p) {
					match2to3.emplace(p, valP);
				});
			}
			if (keyP.size == 3) {
				keyP.foreachVariation([valP](const Pattern& p) {
					match3to4.emplace(p, valP);
				});
			}
		}
		inputStream.close();
		std::vector<Pattern> gridPatterns(1,Pattern(3));
		gridPatterns[0].data = 0b010'001'111;
		auto expanded = expand3(gridPatterns[0]);
		int cnt = 0;
		for (auto i : expanded) {
			cnt += i.count();
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		Pattern p(3);
		p.data = 0b010'001'111;
		std::unordered_map<Pattern, int, PatternHasher> gridPatterns{
			{p, 1}
		};
		//6 because we expand 3 times per iteration
		for (int i=0; i<6; i++) {
			auto temp = gridPatterns;
			gridPatterns.clear();
			for (const auto& [key, val] : temp) {
				auto expanded = expand3(key);
				for (const auto& pat : expanded) {
					gridPatterns[pat] += val;
				}
			}
		}

		int cnt = 0;
		for (const auto& [key, val] : gridPatterns) {
			cnt+=val * key.count();
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
	size progression
	3 -> split 3 -> 4
	4 -> split 2 -> 6
	6 -> split 2 -> 9

	9 -> split 3 -> 12
	12 -> split 2 -> 18
	18 -> split 2 -> 27

	3 * 3^n -> 4 * 3^n -> 6 * 3^n -> 3 * 3^(n+1)
*/