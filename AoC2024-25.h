#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

struct Task
{
	void run() {

		std::ifstream inputStream("input2024-25.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::vector<int>> keys;
		std::vector<std::vector<int>> locks;
		bool isKey = false;
		while (std::getline(inputStream, line))
		{
			if (line.empty())
				break;
			if (line[0] == '#') {
				auto& lock = locks.emplace_back(line.size(), -1);
				while (true) {

					for (int i=0; i<line.size(); i++) {
						if (line[i] == '#')
							lock[i]++;
					}
					if (!std::getline(inputStream, line))
						break;
					if (line.empty())
						break;
				}
			}
			else if (line[0] == '.') {
				auto& key = keys.emplace_back(line.size(), -1);
				while (true) {
					for (int i=0; i<line.size(); i++) {
						if (line[i] == '#')
							key[i]++;
					}

					if (!std::getline(inputStream, line))
						break;
					if (line.empty())
						break;
				}
			}
		}

		int cnt = 0;
		int height = 5;

		for (int i=1; i<keys.size(); i++) {
			for (int j=0; j<i; j++) {
				bool isDuplicate = true;
				for (int k=0; k<keys[i].size(); k++) {
					if (keys[i][k] != keys[j][k])
						isDuplicate = false;
				}
				if (isDuplicate) {
					std::cout << "duplicate!";
				}
			}
		}
		for (int i=1; i<locks.size(); i++) {
			for (int j=0; j<i; j++) {
				bool isDuplicate = true;
				for (int k=0; k<locks[i].size(); k++) {
					if (locks[i][k] != locks[j][k])
						isDuplicate = false;
				}
				if (isDuplicate) {
					std::cout << "duplicate!";
				}
			}
		}

		for (int i=0; i<keys.size(); i++) {
			auto& key = keys[i];

			for (int j = 0; j<locks.size(); j++) {
				auto& lock = locks[j];

				bool suitable = true;
				for (int k=0; k<key.size(); k++) {
					if (key[k] + lock[k] > height) {
						suitable = false;
					}
				}
				if (suitable) {
					cnt++;
				}
			}
		}
		//2836

		std::cout << cnt << '\n';
	}
};
