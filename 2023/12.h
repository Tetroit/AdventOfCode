#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <numeric>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//Fitting patterns in ???

struct Task
{
	static inline std::vector<std::string> schematics;
	static inline std::vector<std::vector<int>> sections;

	template <typename cnt_t>
	static cnt_t nArrangements(const std::string &sch, const std::vector<int> &bars) {
		auto getKey = [](int takenSpace, int barsUsed) -> uint32_t {
			return (takenSpace << 16) | barsUsed;
		};
		std::unordered_map<uint32_t, cnt_t> reg;

		reg[getKey(0, 0)] = 1;
		for (int space = 0; space < sch.size(); ++space) {
			for (int used = 0; used <= bars.size(); used++) {

				uint32_t key = getKey(space, used);
				if (!reg.contains(key)) continue;
				const long long& srcCell = reg[key];

				if (used == bars.size()) {
					if (sch[space] != '#')
						reg[getKey(space+1, used)] += srcCell;
					continue;
				}

				int maxOffset = sch.size() - space - std::accumulate(bars.begin() + used, bars.end(), bars.size() - 1 - used);
				bool stop = false;

				for (int offset = 0; offset <= maxOffset; offset++) {

					if (stop) break;
					if (sch[space + offset] == '#') stop = true;

					bool valid = true;
					for (int i=0; i<bars[used]; i++) {
						if (sch[space + offset + i] == '.') {
							valid = false;
							break;
						}
					}
					int spacePos = space + offset + bars[used];
					int lastPos = spacePos < sch.size() ? spacePos + 1 : spacePos;
					if (spacePos < sch.size() && sch[spacePos] == '#')
						valid = false;

					if (valid) {
						reg[getKey(lastPos, used+1)] += srcCell;
					}
				}
			}
		}
		return reg[getKey(sch.size(), bars.size())];
	}

	static void run() {

		std::ifstream inputStream("2023/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::string schematic;
		std::string sectionsStr;
		while (inputStream >> schematic >> sectionsStr)
		{
			sections.emplace_back();
			auto words = Utils::split(sectionsStr, ',');
			for (const auto& word : words) {
				sections.back().push_back(std::stoi(word));
			}
			schematics.emplace_back(schematic);

		}
		inputStream.close();
		int sum = 0;
		for (int el = 0; el < schematics.size(); ++el) {
			const auto& sch = schematics[el];
			const auto& bars = sections[el];

			int val = nArrangements<int>(sch, bars);
			sum += val;
			// std::cout << val << std::endl;
		}
		std::cout << sum << std::endl;
	}
	static void runPart2() {
		long long sum = 0;
		for (int el = 0; el < schematics.size(); ++el) {
			const auto& sch = schematics[el];
			const auto& bars = sections[el];
			std::string extSch;
			std::vector<int> extBars(bars.size() * 5);
			for (int i=0; i<5; i++) {
				extSch += sch + '?';
				std::memcpy(extBars.data() + i * bars.size(), bars.data(), bars.size() * sizeof(int));
			}
			extSch.pop_back();
			long long val = nArrangements<long long>(extSch, extBars);
			sum += val;
			// std::cout << val << std::endl;
		}
		std::cout << sum << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/