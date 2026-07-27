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

//file explorer simulator

struct Folder {
	std::vector<int> folders;
	std::vector<int> files;
	bool calculated = false;
	int score = -1;
};


struct Task
{
	static inline std::vector<Folder> directory;
	static inline int mainFolder;
	static std::vector<int> readFolders(std::ifstream& inputStream, int N) {
		std::vector<int> ids;
		for (int i=0; i<N; i++) {
			int folders, files;
			inputStream >> folders >> files;
			Folder newFolder;
			if (folders != 0) {
				newFolder.folders = readFolders(inputStream, folders);
			}
			int file;
			for (int i=0; i<files; i++) {
				inputStream >> file;
				newFolder.files.push_back(file);
			}
			ids.push_back(directory.size());
			directory.push_back(newFolder);
		}
		return ids;
	}
	static void CalculateScore(int folderID) {
		Folder& folder = directory[folderID];
		folder.score = 0;
		if (folder.folders.empty()) {
			for (auto& meta : folder.files) {
				folder.score+=meta;
			}
		}
		else {
			for (auto& meta : folder.files) {
				if (meta > 0 && meta <= folder.folders.size()) {
					int childID = folder.folders[meta - 1];
					Folder& child = directory[childID];
					if (!child.calculated)
						CalculateScore(childID);
					folder.score+=child.score;
				}
			}
		}
		folder.calculated = true;
	}
	static void run() {

		std::ifstream inputStream("2018/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		std::vector<int> orders;
		std::vector<int> path;
		int folders, files;
		bool isFolder;
		mainFolder = readFolders(inputStream, 1)[0];
		inputStream.close();
		int cnt = 0;
		for (const auto& folder : directory) {
			for (const auto& file : folder.files) {
				cnt+=file;
			}
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		CalculateScore(mainFolder);
		std::cout << directory[mainFolder].score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/