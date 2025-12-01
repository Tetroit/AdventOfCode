#pragma once

#include <cassert>
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

// The task where you need to parse variables and perform binary operations

struct Task
{
	inline static std::unordered_map<std::string, uint16_t> registry;
	inline static std::unordered_map<std::string, std::vector<std::string>> dependencies;
	inline static std::unordered_map<std::string, std::function<uint16_t(std::vector<uint16_t>)>> functionMap;
	inline static uint16_t overrideA;

	static void calculateWire(
		const std::string& wire,
		std::unordered_map<std::string, uint16_t>& pRegistry,
		const std::unordered_map<std::string, std::vector<std::string>>& pDependencies,
		const std::unordered_map<std::string, std::function<uint16_t(std::vector<uint16_t>)>>& pFunctionMap) {

		//check if calculated
		if (pRegistry.find(wire) != pRegistry.end()) {
			return;
		}
		//calculate dependencies
		std::vector<uint16_t> values{};
		for (const auto& connection : pDependencies.at(wire)) {
			calculateWire(connection, pRegistry, pDependencies, pFunctionMap);
			values.push_back(pRegistry.at(connection));
		}
		//calculate this
		pRegistry[wire] = pFunctionMap.at(wire)(values);
	}

	static void run() {

		std::ifstream inputStream("07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}


		std::regex assignPattern(R"((\d+) -> ([a-z]+))");
		std::regex assignVarPattern(R"(([a-z]+) -> ([a-z]+))");
		std::regex addPattern(R"(([a-z]+) AND ([a-z]+) -> ([a-z]+))");
		std::regex addNumToVarPattern(R"((\d+) AND ([a-z]+) -> ([a-z]+))");
		std::regex orPattern(R"(([a-z]+) OR ([a-z]+) -> ([a-z]+))");
		std::regex rShiftPattern(R"(([a-z]+) RSHIFT (\d+) -> ([a-z]+))");
		std::regex lShiftPattern(R"(([a-z]+) LSHIFT (\d+) -> ([a-z]+))");
		std::regex notPattern(R"(NOT ([a-z]+) -> ([a-z]+))");

		std::smatch match;
		std::string line;
		while (std::getline(inputStream, line))
		{
			bool success = false;
			if (std::regex_match(line, match, assignPattern)) {
				registry[match[2].str()] = std::stoi(match[1].str());
				success = true;
			}
			if (std::regex_match(line, match, assignVarPattern)) {
				functionMap[match[2].str()] = [](const std::vector<uint16_t>& args){return args[0];};
				dependencies[match[2].str()] = std::vector{match[1].str()};
				success = true;
			}
			if (std::regex_match(line, match, addPattern)) {
				functionMap[match[3].str()] = [](const std::vector<uint16_t>& args){return args[0] & args[1];};
				dependencies[match[3].str()] = std::vector{match[1].str(), match[2].str()};
				success = true;
			}
			if (std::regex_match(line, match, addNumToVarPattern)) {
				uint16_t constArg = std::stoi(match[1].str());
				functionMap[match[3].str()] = [constArg](const std::vector<uint16_t>& args){return constArg & args[0];};
				dependencies[match[3].str()] = std::vector{match[2].str()};
				success = true;
			}
			if (std::regex_match(line, match, orPattern)) {
				functionMap[match[3].str()] = [](const std::vector<uint16_t>& args){return args[0] | args[1];};
				dependencies[match[3].str()] = std::vector{match[1].str(), match[2].str()};
				success = true;
			}
			if (std::regex_match(line, match, lShiftPattern)) {
				uint16_t constArg = std::stoi(match[2].str());
				functionMap[match[3].str()] = [constArg](const std::vector<uint16_t>& args){return args[0] << constArg;};
				dependencies[match[3].str()] = std::vector{match[1].str()};
				success = true;
			}
			if (std::regex_match(line, match, rShiftPattern)) {
				uint16_t constArg = std::stoi(match[2].str());
				functionMap[match[3].str()] = [constArg](const std::vector<uint16_t>& args){return args[0] >> constArg;};
				dependencies[match[3].str()] = std::vector{match[1].str()};
				success = true;
			}
			if (std::regex_match(line, match, notPattern)) {
				functionMap[match[2].str()] = [&](const std::vector<uint16_t>& args){return ~args[0];};
				dependencies[match[2].str()] = std::vector{match[1].str()};
				success = true;
			}
			assert(success == true);
		}
		calculateWire("a", registry, dependencies, functionMap);
		overrideA = registry.at("a");
		std::cout << registry.at("a") << '\n';
	}
	static void runPart2() {
		for (const auto& connection : dependencies) {
			if (auto it = registry.find(connection.first); it != registry.end()) {
				registry.erase(registry.find(connection.first));
			}
		}
		registry["b"] = overrideA;
		calculateWire("a", registry, dependencies, functionMap);
		std::cout << registry["a"] << '\n';
	}
};

//-------------- NOTES AREA ----------------
/*
So I couldve done it normally but for whatever reason I wanted to use lambda exprs
*/