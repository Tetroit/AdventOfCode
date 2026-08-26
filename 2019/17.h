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

#include "GridBase.h"
#include "Intcode.h"
#include "utils.h"

//traversing map from intcode generated image

struct Task
{
	static inline DynamicGrid<char> image;
	static inline Intcode<int> program;
	static void run() {

		std::ifstream inputStream("2019/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		program = Intcode<int>(inputStream);

		inputStream.close();

		std::deque<int> input;
		std::deque<int> output;
		HaltCode code;

		program.start();
		while (program.execute(input,output,code));
		program.end();

		int readX = 0, readY = 0;
		for (int i : output) {
			if (i == 10) {
				readX = 0;
				readY++;
				continue;
			}
 			image.add(readX, readY, char(i));
			readX++;
		}
		int score = 0;
		image.foreachCoord([&](int x, int y) {
			if (image.countNeighbours4(x,y,[](char ch) {return ch == '#';}) == 4) {
				score += x*y;
				// image.set(x,y,'O');
			}
		});
		// image.print(DefaultConvert<char>());
		std::cout << score << std::endl;
	}
	static void stringReinterpret(std::deque<int>& input, std::string text) {
		for (int i=0; i<text.length(); i++) {
			input.push_back(static_cast<int>(text[i]));
		}
		input.push_back(10);
	}
	static void runPart2() {
		int botX, botY, botDir;
		image.foreachCoord([&](int x, int y) {
			char symb = image.get(x,y);
			if (symb == 'v' || symb == '>' || symb == '<' || symb == '^') {
				botX = x;
				botY = y;
				switch (symb) {
					case '>': botDir = 0; break;
					case '^': botDir = 1; break;
					case '<': botDir = 2; break;
					case 'v': botDir = 3; break;
				}
			}
		});
		std::string path;
		int fwd = 0;
		while (true) {
			auto next = image.getInDir(botX, botY, botDir);
			if (next.has_value() && next.value() == '#') {
				fwd++;
				auto[dx, dy] = Utils::UtilFacingVec[botDir];
				botX += dx;
				botY += dy;
				continue;
			}
			if (fwd != 0)
				path+=std::to_string(fwd) + ',';
			fwd = 0;
			int rDir = (botDir+3)%4;
			int lDir = (botDir+1)%4;
			if (image.getInDir(botX, botY, rDir) == '#') {
				botDir = rDir;
				path+="R,";
				continue;
			}
			if (image.getInDir(botX, botY, lDir) == '#') {
				botDir = lDir;
				path+="L,";
				continue;
			}
			break;
		}
		path.pop_back();
		std::cout << path << std::endl;
		
		std::deque<int> input;
		std::deque<int> output;
		HaltCode code;

		program.program[0]=2;
		program.start();
		std::string console;
		std::vector<std::string> stages{
			"Insert command sequence (A/B/C)",
			"Insert moveset A (R/L/<n>)",
			"Insert moveset B (R/L/<n>)",
			"Insert moveset C (R/L/<n>)",
			"Video replay? (y/n)"
		};
		bool videoReplay = false;
		// for (int i=0; i<5; i++){
		// 	while (program.executeUntilInput(input, output, code));
		// 	std::cout << stages[i] << std::endl;
		// 	std::cin >> console;
		// 	stringReinterpret(input, console);
		// 	if (i == 4 && console == "y") videoReplay = true;
		// }
		stringReinterpret(input, "A,A,B,C,A,C,A,B,C,B");
		stringReinterpret(input, "R,12,L,8,R,6");
		stringReinterpret(input, "R,12,L,6,R,6,R,8,R,6");
		stringReinterpret(input, "L,8,R,8,R,6,R,12");
		stringReinterpret(input, videoReplay ? "y" : "n");

		while (program.execute(input, output, code));
		program.end();
		while (output.size() > 1) {
			std::cout << char(output.front());
			output.pop_front();
		}
		std::cout.flush();
		std::cout << output.front() << std::endl;
	}
};


//-------------- NOTES AREA ----------------
/*

R,12,L,8,R,6,
R,12,L,8,R,6,
R,12,L,6,R,6,R,8,R,6,
L,8,R,8,R,6,R,12,
R,12,L,8,R,6,
L,8,R,8,R,6,R,12,
R,12,L,8,R,6,
R,12,L,6,R,6,R,8,R,6,
L,8,R,8,R,6,R,12,
R,12,L,6,R,6,R,8,R,6

A: R,12,L,8,R,6
B: R,12,L,6,R,6,R,8,R,6
C: L,8,R,8,R,6,R,12

A,A,B,C,A,C,A,B,C,B
*/