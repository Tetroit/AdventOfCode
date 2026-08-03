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

//assembly simulator with function ptrs

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

		std::ifstream inputStream("2018/19.txt");

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
		while (true) {
			int commandID = arr[ip];
			if (commandID >= program.size()  || commandID < 0) break;
			auto& [cmd, a, b, c] = program[commandID];
			Execute(cmd, arr, a, b, c);
			arr[ip]++;
		}
		std::cout << arr[0] <<  std::endl;
	}
	static void runPart2() {

		long long cnt = 0;
		int goal = 10551386;
		for (int a5 = 1; a5 <= goal; a5++ ) {
			if (goal % a5 == 0) {
				cnt += a5;
			}
		}
		std::cout << cnt << std::endl;
		//39967680
	}
};

//-------------- NOTES AREA ----------------
/*

0 addi 4 16 4	f+=16 ->

1 seti 1 3 5	for (a5 = 0; a5 <= a2; a5++) <- //a2 = 10551386 and doesnt change
2 seti 1 1 3		for (a3 = 0; a3 <= a2; a3++) { <-
3 mulr 5 3 1
4 eqrr 1 2 1		if (a5*a3 == a2)
5 addr 1 4 4
6 addi 4 1 4
7 addr 5 0 0			a0 += a5
8 addi 3 1 3
9 gtrr 3 2 1
10 addr 4 1 4
11 seti 2 8 4		} ->
12 addi 5 1 5
13 gtrr 5 2 1
14 addr 1 4 4
15 seti 1 3 4	} ->
16 mulr 4 4 4	return

17 addi 2 2 2	a2+=2 <-
18 mulr 2 2 2	a2=sqr(a2)
19 mulr 4 2 2	a2*=19 (f=19)
20 muli 2 11 2	a2*=11
21 addi 1 6 1	a1+=6
22 mulr 1 4 1	a1*=4
23 addi 1 18 1	a1+=18
24 addr 2 1 2	a2+=a1
25 addr 4 0 4	f+=1
26 seti 0 3 4	f=0 -> //part 1 a2 = 986
27 setr 4 5 1	a1=27
28 mulr 1 4 1	a1*=28;
29 addr 4 1 1	a1+=29;
30 mulr 4 1 1	a1*=30;
31 muli 1 14 1	a1*=14;
32 mulr 1 4 1	a1*=32;
33 addr 2 1 2	a2+=a1
34 seti 0 1 0
35 seti 0 4 4	f=0 -> //part 2 a2 = 10551386
*/