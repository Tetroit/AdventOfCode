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

//navigating file directory

struct Task
{
	static inline std::unordered_map<std::string, std::vector<std::string>> fileGraph;
	static inline std::unordered_map<std::string, int> sizes;
	static inline std::vector<std::string> dirs{{"/"}};

	static inline const std::regex dirPat = std::regex(R"(dir (\w+))");
	static inline const std::regex filePat = std::regex(R"((\d+) (.+))");


	static void countFolderSize(const std::string& path) {
		int size = 0;
		for (const auto& child : fileGraph[path]) {
			auto childPath = path + "/" + child;
			if (!sizes.contains(childPath))
				countFolderSize(childPath);
			size+=sizes.at(childPath);
		}
		sizes[path] = size;
	}
	static void run() {

		std::ifstream inputStream("2022/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::vector<std::string> path;
		bool fileList = false;
		while (std::getline(inputStream, line))
		{
			if (line[0] == '$') {
				fileList = false;
				std::string command = line.substr(2, 2);
				if (command == "cd") {
					std::string dst = line.substr(5, line.size() - 5);
					if (dst == "..") path.pop_back();
					else path.push_back(dst);
					continue;
				}
				if (command == "ls") {
					fileList = true;
					continue;
				}
			}
			else {
				std::smatch result;
				std::string parentStr;
				for (auto& fold : path)
					parentStr += fold + "/";
				parentStr.pop_back();

				if (std::regex_match(line, result, dirPat)) {

					std::string pathStr = parentStr + '/' + result[1].str();

					fileGraph[parentStr].push_back(result[1].str());
					dirs.push_back(pathStr);
				}
				else if (std::regex_match(line, result, filePat)) {

					std::string pathStr = parentStr + '/' + result[2].str();

					fileGraph[parentStr].push_back(result[2].str());
					sizes[pathStr] += std::stoi(result[1]);
				}
			}
		}
		inputStream.close();
		countFolderSize("/");
		int cnt = 0;

		for (const auto& [entry, size] : sizes) {
			std::cout << entry << ": " << size << std::endl;
		}
		for (const auto& entry : dirs) {
			if (sizes[entry] <= 100000) cnt+=sizes[entry];
		}
		std::cout << cnt << std::endl;
		//168234 low
	}
	static void runPart2() {
		static const int totalSpace = 70000000;
		static const int neededSpace = 30000000;
		int freeSpace = totalSpace - sizes["/"];
		int toDelete = neededSpace - freeSpace;
		int min = INT_MAX;
		for (auto dir : dirs) {
			int size = sizes[dir];
			if (size < toDelete) continue;
			if (size < min) min = size;
		}
		std::cout << min << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/