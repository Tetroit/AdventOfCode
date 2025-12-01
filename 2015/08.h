#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

// The task to parse string using escape codes

struct Task
{
	static void run() {

		std::ifstream inputStream("08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		int rawScore = 0;
		int decodedScore = 0;
		while (std::getline(inputStream, line))
		{
			std::string decoded = "";
			for (int i=1; i<line.size() - 1; i++) {
				if (line[i] == '\\') {
					i++;
					if (line[i] == 'x') {
						i++;
						decoded += (char) std::stoi(line.substr(i, 2), nullptr, 16);
						i++;
					}
					else {
						decoded += line[i];
					}
				}
				else {
					decoded += line[i];
				}
			}
			rawScore += line.length();
			decodedScore += decoded.length();
		}
		std::cout<<rawScore-decodedScore<<"\n";
		inputStream.close();
	}
	//1472
	static void runPart2() {
		std::ifstream inputStream("08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		int score = 0;
		while (std::getline(inputStream, line))
		{
			for (char i : line) {
				if (i == '\\' || i == '\"') {
					score++;
					//cheating
				}
			}
			score+=2;
			//more cheating
		}
		std::cout<<score<<"\n";
		inputStream.close();
	}
};

//-------------- NOTES AREA ----------------
/*

*/