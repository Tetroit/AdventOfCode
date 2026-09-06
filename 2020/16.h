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

//Checking train tickets

struct Ranges {
	int minA;
	int maxA;
	int minB;
	int maxB;
	bool has(int i) const {
		if ((i >= minA and i <= maxA) or (i >= minB and i <= maxB)) {
			return true;
		}
		return false;
	}
};
struct Task
{
	static inline std::unordered_map<std::string, Ranges> fields;
	static inline std::vector<int> yourTicket;
	static inline std::vector<std::vector<int>> tickets;
	static void run() {

		std::regex fieldPattern  (R"((.+): (\d+)-(\d+) or (\d+)-(\d+))");

		std::ifstream inputStream("2020/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		bool inputYourTicket = false;
		bool inputTickets = false;
		while (std::getline(inputStream, line))
		{
			if (!inputTickets && !inputYourTicket) {
				std::smatch match;
				if (std::regex_match(line, match, fieldPattern)) {
					auto key = match[1].str();
					Ranges ranges;
					ranges.minA = std::stoi(match[2].str());
					ranges.maxA = std::stoi(match[3].str());
					ranges.minB = std::stoi(match[4].str());
					ranges.maxB = std::stoi(match[5].str());
					fields[key] = ranges;
				}
			}
			if (line == "your ticket:") {
				inputYourTicket = true;
				continue;
			}
			if (line == "nearby tickets:") {
				inputTickets = true;
				inputYourTicket = false;
				continue;
			}
			if (inputYourTicket) {
				if (line.empty()) continue;
				std::istringstream iss(line);
				int val;
				while (iss >> val) {
					iss.ignore(1);
					yourTicket.push_back(val);
				}
			}
			else if (inputTickets) {
				if (line.empty()) continue;
				tickets.emplace_back();
				auto& newTicket = tickets.back();
				std::istringstream iss(line);
				int val;
				while (iss >> val) {
					iss.ignore(1);
					newTicket.push_back(val);
				}
			}
		}
		inputStream.close();

		int cnt = 0;
		for (int i = tickets.size() - 1; i >= 0; i--) {
			auto& ticket = tickets[i];
			bool validTicket = true;
			for (auto& value : ticket) {
				bool valid = false;
				for (auto& [name, range] : fields) {
					if (range.has(value)) {
						valid = true;
						break;
					}
				}
				if (!valid) {
					cnt+=value;
					validTicket = false;
				}
			}
			if (!validTicket) {
				tickets.erase(tickets.begin() + i);
			}
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::vector<std::string> allFields;
		for (const auto& [name, range] : fields) {
			allFields.push_back(name);
		}
		std::vector<std::vector<std::string>> validFields(fields.size(), allFields);
		for (auto& ticket : tickets) {
			for (int i = 0; i < fields.size(); i++) {
				const auto& value = ticket[i];
				auto& candidateFields = validFields[i];
				for (int j = candidateFields.size() - 1; j >= 0; j--) {
					const auto& field = candidateFields[j];
					if (!fields[field].has(value)) {
						candidateFields.erase(candidateFields.begin() + j);
					}
				}
			}
		}
		std::unordered_map<std::string, int> fieldMap;
		for (int loop=0; loop < fields.size(); loop++) {
			for (int i=0; i < fields.size(); i++) {
				auto& candidateFields = validFields[i];

				if (candidateFields.size() != 1) {
					for (int j = candidateFields.size() - 1; j >= 0; j--) {
						if (fieldMap.contains(candidateFields[j]))
							candidateFields.erase(candidateFields.begin() + j);
					}
				}
				if (candidateFields.size() == 1) {
					fieldMap.emplace(candidateFields[0], i);
					if (fieldMap.size() == fields.size()) {
						break;
					}
				}
			}
		}
		long long res = 1;
		for (auto [name, id]  : fieldMap) {
			if (name.substr(0, 9) == "departure") {
				res = res * yourTicket[id];
			}
		}
		std::cout << res << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/