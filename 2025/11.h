#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//Task about counting paths that go through 2 specific nodes
struct Task
{

	static void run() {

		std::ifstream inputStream("2025/input/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::unordered_map<std::string, std::vector<std::string>> map;
		std::unordered_map<std::string, int> writeCount;
		while (std::getline(inputStream, line))
		{
			int separation = line.find(':');
			std::string input = line.substr(0, separation);
			std::string output = line.substr(separation + 2);
			auto words = Utils::split(output, ' ');
			map.emplace(input, words);
		}
		inputStream.close();


		std::vector<std::string> stack;
		writeCount["you"] = 1;
		stack.emplace_back("you");
		//very basic DP approach, each visit a vertex, add +1 to all its children
		//add each child to stack to go through it again (even if we visited them already, im too lazy to cache it)
		while (!stack.empty()) {
			std::string key = stack.back();
			stack.pop_back();
			for (auto& connection: map[key]) {
				writeCount[connection]++;
				//terminate out
				if (connection != "out")
					stack.push_back(connection);
			}
		}
		//in the end every node would count +1 for each path to out
		std::cout << writeCount["out"] << std::endl;
	}

	//part 2
	//for each node we will keep track of how many paths lead through fft, dac, fft&dac, and none of them
	struct CacheData {
		bool hasFft = false;
		bool hasDac = false;
		long long outCountNone = 0;
		long long outCountFft = 0;
		long long outCountDac = 0;
		long long outCountBoth = 0;
	};
	static void MergeData(CacheData& to, const CacheData& from) {
		to.hasFft |= from.hasFft;
		to.hasDac |= from.hasDac;
		to.outCountNone += from.outCountNone;
		to.outCountFft += from.outCountFft;
		to.outCountDac += from.outCountDac;
		to.outCountBoth += from.outCountBoth;
	}
	static CacheData depthSearch(
		const std::unordered_map<std::string, std::vector<std::string>>& map,
		std::vector<std::string>& path,
		std::unordered_map<std::string, CacheData>& cache) {

		//we will go down the tree while also caching each node we calculate,
		//so that if we visit this node again
		//we know for sure how many out paths it has and we dont recurse again
		CacheData cacheData;
		for (auto& connection: map.at(path.back())) {
			//checking children:
			//count number of outs
			//count towards none because out itself is not fft or dac or both
			if (connection == "out") {
				cacheData.outCountNone++;
				continue;
			}
			CacheData childData;
			//go deeper in recursion if none was found
			if (cache.find(connection) == cache.end()) {
				path.push_back(connection);
				childData = depthSearch(map, path, cache);
				path.pop_back();
			}
			//or retrieve data from cache if it was
			else {
				childData = cache.at(connection);
			}
			//add data to the current connection
			MergeData(cacheData, childData);
		}
		//if we happen to be fft or dac node, we need to count every path that it has fft in it
		//none => fft
		//dac => fft & dac

		//NOTE fft and both count will be certainly 0,
		//as we know we haven't visited any fft node yet, otherwise recursion would not reach here
		//so, basically, each of these ifs is reached only once

		if (path.back() == "fft") {
			cacheData.hasFft = true;
			cacheData.outCountFft = cacheData.outCountNone;
			cacheData.outCountBoth = cacheData.outCountDac;
		//set dac and none to 0 as every path has fft for sure
			cacheData.outCountDac = 0;
			cacheData.outCountNone = 0;
		}
		if (path.back() == "dac") {
			cacheData.hasDac = true;
			cacheData.outCountDac = cacheData.outCountNone;
			cacheData.outCountBoth = cacheData.outCountFft;
			cacheData.outCountFft = 0;
			cacheData.outCountNone = 0;
		}
		//record to cache
		cache.emplace(path.back(), cacheData);
		return cacheData;
	}
	static void runPart2() {

		std::ifstream inputStream("2025/input/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::unordered_map<std::string, std::vector<std::string>> map;
		std::unordered_map<std::string, CacheData> nodeCache;
		std::vector<std::string> path;

		while (std::getline(inputStream, line))
		{
			int separation = line.find(':');
			std::string input = line.substr(0, separation);
			std::string output = line.substr(separation + 2);
			auto words = Utils::split(output, ' ');
			map.emplace(input, words);
		}
		inputStream.close();
		//start our path from svr
		path.emplace_back("svr");
		//start recursion
		auto data = depthSearch(map, path, nodeCache);
		std::cout << data.outCountBoth << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/