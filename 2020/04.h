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

//Checking if all fields are present

struct Task
{
	static inline std::vector<std::unordered_map<std::string, std::string>> data;
	static void run() {

		std::ifstream inputStream("2020/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int n = 0;
		data.emplace_back();
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				n++;
				data.emplace_back();
				continue;
			}
			auto entries = Utils::split(line, ' ');
			for (auto& entry : entries) {
				data.back()[entry.substr(0, 3)] = entry.substr(4);
			}
		}
		inputStream.close();
		int correct = 0;
		for (auto& entry : data) {
			if (entry.size() == 8) correct++;
			else if (entry.size() == 7 && !entry.contains("cid")) correct++;
		}
		std::cout << correct << std::endl;
		//105 low
	}
	static bool check(std::unordered_map<std::string, std::string>& entry) {
		if (entry.size() < 7) return false;
		if (entry.size() == 7 && entry.contains("cid")) return false;

		auto checkYear = [](int min, int max, std::string key) {
			if (key.size() != 4) return false;
			for (int i=0; i<4; i++) {
				if (!std::isdigit(key[i])) return false;
			}
			int byr = std::stoi(key);
			if (byr < min || byr > max) return false;
			return true;
		};

		auto checkCol = [](std::string key) {
			if (key.size() != 7) return false;
			if (key[0] != '#') return false;
			for (int i=1; i<7; i++) {
				if (!std::isdigit(key[i]) && (key[i] < 'a' || key[i] > 'f')) return false;
			}
			return true;
		};

		std::string key;
		if (!checkYear(1920, 2002, entry["byr"])) return false;
		if (!checkYear(2010, 2020, entry["iyr"])) return false;
		if (!checkYear(2020, 2030, entry["eyr"])) return false;

		key = entry["hgt"];
		if (key.size() < 3) return false;
		std::string units = key.substr(key.size() - 2);
		if (units != "in" && units != "cm") return false;
		std::string val = key.substr(0, key.size() - 2);
		for (char i : val) {
			if (!std::isdigit(i)) return false;
		}
		int hgt = std::stoi(val);
		if (units == "in" && (hgt < 59 || hgt > 76)) return false;
		if (units == "cm" && (hgt < 150 || hgt > 193)) return false;

		if (!checkCol(entry["hcl"])) return false;

		const std::unordered_set<std::string> colors = {
			"amb", "blu", "brn", "gry", "grn", "hzl", "oth"
		};
		if (!colors.contains(entry["ecl"])) return false;

		key = entry["pid"];
		if (key.size() != 9) return false;
		for (int i=0; i<9; i++) {
			if (!std::isdigit(key[i])) return false;
		}
		return true;
	}
	static void runPart2() {
		int cnt = 0;
		for (auto& entry : data) {
			if (check(entry)) cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/