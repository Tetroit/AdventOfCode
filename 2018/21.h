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

//assembly simulator with some shenanigans


enum struct Command {
	addr = 0,
	addi = 1,
	mulr = 2,
	muli = 3,
	banr = 4,
	bani = 5,
	borr = 6,
	bori = 7,
	setr = 8,
	seti = 9,
	gtir = 10,
	gtri = 11,
	gtrr = 12,
	eqir = 13,
	eqri = 14,
	eqrr = 15,
};
struct Task
{
	static constexpr Command fromString(std::string& str) {
		if (str == "addr") return Command::addr;
		if (str == "addi") return Command::addi;
		if (str == "mulr") return Command::mulr;
		if (str == "muli") return Command::muli;
		if (str == "banr") return Command::banr;
		if (str == "bani") return Command::bani;
		if (str == "borr") return Command::borr;
		if (str == "bori") return Command::bori;
		if (str == "setr") return Command::setr;
		if (str == "seti") return Command::seti;
		if (str == "gtir") return Command::gtir;
		if (str == "gtri") return Command::gtri;
		if (str == "gtrr") return Command::gtrr;
		if (str == "eqir") return Command::eqir;
		if (str == "eqri") return Command::eqri;
		if (str == "eqrr") return Command::eqrr;
		throw std::runtime_error("Unknown command");
	}

	static void Execute(Command cmd, std::vector<int>& reg, int a, int b, int c) {
		switch (cmd) {
			case Command::addr: {
				reg[c] = reg[a] + reg[b];
				break;
			}
			case Command::addi: {
				reg[c] = reg[a] + b;
				break;
			}
			case Command::mulr: {
				reg[c] = reg[a] * reg[b];
				break;
			}
			case Command::muli: {
				reg[c] = reg[a] * b;
				break;
			}
			case Command::banr: {
				reg[c] = reg[a] & reg[b];
				break;
			}
			case Command::bani: {
				reg[c] = reg[a] & b;
				break;
			}
			case Command::borr: {
				reg[c] = reg[a] | reg[b];
				break;
			}
			case Command::bori: {
				reg[c] = reg[a] | b;
				break;
			}
			case Command::setr: {
				reg[c] = reg[a];
				break;
			}
			case Command::seti: {
				reg[c] = a;
				break;
			}
			case Command::gtir: {
				reg[c] = a > reg[b] ? 1 : 0;
				break;
			}
			case Command::gtri: {
				reg[c] = reg[a] > b ? 1 : 0;
				break;
			}
			case Command::gtrr: {
				reg[c] = reg[a] > reg[b] ? 1 : 0;
				break;
			}
			case Command::eqir: {
				reg[c] = a == reg[b] ? 1 : 0;
				break;
			}
			case Command::eqri: {
				reg[c] = reg[a] == b ? 1 : 0;
				break;
			}
			case Command::eqrr: {
				reg[c] = reg[a] == reg[b] ? 1 : 0;
				break;
			}
		}
	}
	static inline std::vector<std::tuple<Command,int,int,int>> program;
	static inline int ip;
	static void run() {

		std::ifstream inputStream("2018/21.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int val;
		inputStream.ignore(4);
		inputStream >> val;
		inputStream.ignore(1);
		ip = val;
		{
			std::string code;
			int a, b, c;
			Command command;
			while (std::getline(inputStream, line))
			{
				std::istringstream iss(line);
				iss >> code >> a >> b >> c;
				command = fromString(code);
				program.emplace_back(command, a, b, c);
			}
		}
		inputStream.close();
		std::vector<int> arr(6,0);
		std::unordered_set<int> cache;
		int generated = 0;
		int previous = 0;
		while (true) {
			int commandID = arr[ip];
			if (commandID == 28) {
				std::cout << arr[3] << std::endl;
				return;
			}
			if (commandID >= program.size()  || commandID < 0) break;
			auto& [cmd, a, b, c] = program[commandID];
			Execute(cmd, arr, a, b, c);
			arr[ip]++;
		}
		std::cout << arr[0] <<  std::endl;
		//14104079
	}
	static void runPart2() {
		std::vector<int> arr(6,0);
		std::unordered_set<int> cache;
		int generated = 0;
		int previous = 0;
		while (true) {
			int commandID = arr[ip];
			if (commandID == 28) {
				cache.insert(arr[3]);
				generated++;
				if (generated != cache.size()) {
					std::cout << previous << std::endl;
					return;
				}
				previous = arr[3];
			}
			if (commandID >= program.size()  || commandID < 0) break;
			auto& [cmd, a, b, c] = program[commandID];
			Execute(cmd, arr, a, b, c);
			arr[ip]++;
		}
		std::cout << arr[0] <<  std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
0  seti 123 0 3			a3 = 123
1  bani 3 456 3			a3 = a3 & 456
2  eqri 3 72 3			a3 = 1 //123 & 456 =72
3  addr 3 2 2			f += 1
4  seti 0 0 2			//skip
5  seti 0 4 3			a3 = 0
6  bori 3 65536 4		a4 = a3 | 0x10000
7  seti 1107552 3 3		a3 = 1107552
8  bani 4 255 5			a5 = a4 | 0xff
9  addr 3 5 3			a3 += a5
10 bani 3 16777215 3	a3 &= 0xffffff
11 muli 3 65899 3		a3 *= 65899
12 bani 3 16777215 3	a3 &= 0xffffff
13 gtir 256 4 5			a5 = 256 > a4
14 addr 5 2 2			f += a5
15 addi 2 1 2			f++
16 seti 27 0 2			f = 28
17 seti 0 2 5			//some random stuff idk
18 addi 5 1 1
19 muli 1 256 1
20 gtrr 1 4 1
21 addr 1 2 2
22 addi 2 1 2
23 seti 25 3 2
24 addi 5 1 5
25 seti 17 3 2
26 setr 5 3 4
27 seti 7 4 2
28 eqrr 3 0 5			a5 = a3 == a0 //THE ONLY USE OF a0 HERE
29 addr 5 2 2			return
30 seti 5 8 2			f = 6


assuming it loops somewhere maximum is the last number before that loop

*/