#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"


//Tasks
struct Task
{
	static int GetPatternSize(unsigned pattern) {
		int size = 0;
		while (pattern != 0) {
			if ((pattern & 1u) == 1u)
				size++;
			pattern >>= 1;
		}
		return size;
	}
	static void AddJoltage(std::vector<int>& joltage, unsigned pattern, int count = 1) {
		int pos = 0;
		while (pattern != 0) {
			if ((pattern & 1u) == 1u)
				joltage[pos] += count;
			pattern >>= 1;
			pos++;
		}
	}
	static int GetAsNumber(const std::vector<int>& pattern) {
		int number = 0;
		for (int i : pattern){
			if (i > 0)
				number++;
			number <<= 1;
		}
		return number;
	}
	static int NDot(const std::vector<int>& v1, const std::vector<int>& v2) {
		if (v1.size() != v2.size())
			throw std::invalid_argument("Wrong size of vectors");
		int res = 0;
		for (int i = 0; i < v1.size(); i++) {
			if (v1[i] <= 0 || v2[i] <= 0)
				continue;
			res += v1[i] * v2[i];
		}
		return res;
	}
	static int NMagnitude(const std::vector<int>& v) {
		int n = 0;
		for (int i : v) {
			if (i > 0)
				n++;
		}
		return n;
	}
	static void NSubtract(std::vector<int>& v1, const std::vector<int>& v2) {
		if (v1.size() != v2.size())
			throw std::invalid_argument("Wrong size of vectors");
		for (int i=0; i<v1.size(); i++) {
			v1.at(i) -= v2.at(i);
		}
	}
	static bool IsZero(const std::vector<int>& v) {
		for (const int& i : v) {
			if (i != 0)
				return false;
		}
		return true;
	}
	static bool AllNonPositive(const std::vector<int>& v) {
		for (const int& i : v) {
			if (i > 0)
				return false;
		}
		return true;
	}
	struct Machine {
		unsigned lights;
		int lightCount;
		std::vector<std::vector<int>> schemes;
		std::vector<int> joltage;

		void AddSchemeFromIds(const std::vector<int>& ids) {
			std::vector<int> vec(lightCount, 0);
			for (int id : ids) {
				vec[id]++;
			}
			schemes.push_back(vec);
		}
	};
	static void run() {

		std::ifstream inputStream("2025/input/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<Machine> machines;
		while (std::getline(inputStream, line))
		{
			auto& machine = machines.emplace_back();
			int schemeStart = 1;
			int schemeEnd = line.find(']');
			unsigned lights = 0;
			int digit = 0;
			for (int i=schemeStart; i<schemeEnd; i++) {
				if (line[i] == '#')
					lights += 1<<digit;
				digit++;
			}
			machine.lights = lights;
			machine.lightCount = digit;
			while (true) {
				std::vector<int> ids;
				schemeStart = line.find('(', schemeEnd + 1);
				schemeEnd = line.find(')', schemeEnd + 1);
				if (schemeEnd == std::string::npos)
					break;
				auto scheme = line.substr(schemeStart + 1, schemeEnd - schemeStart-1);
				auto words = Utils::split(scheme, ',');
				for (auto& word : words) {
					ids.push_back(std::stoi(word));
				}
				machine.AddSchemeFromIds(ids);
			}
		}
		inputStream.close();
		int total = 0;
		for (auto& machine : machines) {
			int min = std::numeric_limits<int>::max();
			for (int i=0; i< 1 << machine.schemes.size(); i++) {
				int cnt = 0;
				unsigned combo = 0;
				for (int j=0; j < machine.schemes.size(); j++) {
					if ((i & 1<<j) != 0) {
						cnt++;
						combo ^= GetAsNumber(machine.schemes[j]);
					}
				}
				if (combo == machine.lights && cnt < min) {
					min = cnt;
				}
			}
			total += min;
		}
		std::cout << total << "\n";
	}
	static void runPart2() {
		std::ifstream inputStream("2025/input/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<Machine> machines;
		while (std::getline(inputStream, line))
		{
			auto& machine = machines.emplace_back();
			int schemeStart = 1;
			int schemeEnd = line.find(']');
			unsigned lights = 0;
			int digit = 0;
			for (int i=schemeStart; i<schemeEnd; i++) {
				if (line[i] == '#')
					lights += 1<<digit;
				digit++;
			}
			machine.lights = lights;
			machine.lightCount = digit;
			while (true) {
				std::vector<int> ids;
				schemeStart = line.find('(', schemeEnd + 1);
				schemeEnd = line.find(')', schemeEnd + 1);
				if (schemeEnd == std::string::npos)
					break;
				auto scheme = line.substr(schemeStart + 1, schemeEnd - schemeStart-1);
				auto words = Utils::split(scheme, ',');
				for (auto& word : words) {
					ids.push_back(std::stoi(word));
				}
				machine.AddSchemeFromIds(ids);
			}
			schemeStart = line.find('{', schemeEnd + 1);
			schemeEnd = line.find('}', schemeEnd + 1);
			auto scheme = line.substr(schemeStart + 1, schemeEnd - schemeStart-1);
			auto words = Utils::split(scheme, ',');
			for (auto& word : words) {
				machine.joltage.push_back(std::stoi(word));
			}
		}
		inputStream.close();
		int total = 0;
		for (auto& machine : machines) {
			std::vector<int> vectorAmounts(machine.schemes.size(), 0);
			std::vector<int> jCopy = machine.joltage;
			while (!AllNonPositive(jCopy)) {
				int closestID = -1;
				float maxDot = -10000;
				for (int i=0; i<vectorAmounts.size(); i++) {
					const auto& currentScheme = machine.schemes.at(i);
					float dot = (float)NDot(currentScheme, jCopy);
					if (dot > maxDot
						//|| dot == maxDot && NMagnitude(machine.schemes.at(closestID)) > NMagnitude(machine.schemes.at(i))
					){
						closestID = i;
						maxDot = dot;
					}
				}
				vectorAmounts[closestID]++;
				NSubtract(jCopy, machine.schemes.at(closestID));
			}
			for (int i : vectorAmounts) {
				std::cout << i << " ";
				total += i;
			}
			std::cout << "total: " << total << "\n";
			//15755
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/