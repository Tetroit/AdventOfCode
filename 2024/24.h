#pragma once

#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

struct Task
{
	enum struct Operation {
		AND,
		OR,
		XOR,
		VALUE
	};

	struct Gate {
		std::string a;
		std::string b;
		Operation op;
		bool value = false;
		bool calculated = false;
	};

	std::unordered_map<std::string, Gate> gates;


	std::string getOpName(Operation o) {
		if (o == Operation::OR) {
			return "|";
		}
		if (o == Operation::XOR) {
			return "^";
		}
		if (o == Operation::AND) {
			return "&";
		}
		return "";
	}
	bool evaluateGate(Gate& gate) {
		if (gate.op == Operation::VALUE  || gate.calculated) {
			return gate.value;
		}

		std::cout << " (" << gate.a;

		auto val1 = evaluateGate(gates[gate.a]);

		std::cout << " " << getOpName(gate.op) << " " << gate.b;

		auto val2 = evaluateGate(gates[gate.b]);

		std::cout << ")";

		if (gate.op == Operation::OR) {
			gate.value = (val1 || val2);
		}
		else if (gate.op == Operation::XOR) {
			gate.value = (val1 != val2);
		}
		else if (gate.op == Operation::AND) {
			gate.value = (val1 && val2);
		}
		gate.calculated = true;
		return gate.value;
	}

	void swapWires (const std::string& a, const std::string& b) {
		std::swap(gates[a], gates[b]);
	}

	uint64_t calculate(int size) {
		std::bitset<64> bits(0);
		for (int i=0; i < size; i++) {
			std::string code = i > 9 ? std::to_string(i) : "0" + std::to_string(i);
			std::string key = "z" + code;
			std::cout << key << ": ";

			auto& gate = gates[key];
			int bit;
			try {
				bit = std::stoi(key.substr(1));
			}
			catch (...) {
				continue;
			}
			int res = evaluateGate(gate);
			std::cout << '\n';

			bits[bit] = res;
		}

		for (auto& [key, gate] : gates) {
			gate.calculated = false;
		}
		return bits.to_ullong();
	}

	uint64_t calculate(uint64_t a, uint64_t b, int size) {
		std::bitset<64> bitsA(a);
		std::bitset<64> bitsB(b);
		for (int i = 0; i < size; i++) {
			std::string code = i > 9 ? std::to_string(i) : "0" + std::to_string(i);
			gates["x" + code].value = bitsA[i];
			gates["y" + code].value = bitsB[i];
		}
		return calculate(size);
	}

	void run() {

		std::ifstream inputStream("input2024-24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			if (line.empty())
				break;
			std::string code = line.substr(0, 3);
			std::stringstream ss(line.substr(line.find(':') + 1));
			bool val;
			ss >> val;
			gates[code] = {"", "", Operation::VALUE, val, false};
		}
		while (std::getline(inputStream, line)){
			auto words = split(line, ' ');
			Operation op;
			if (words[1] == "OR")
				op = Operation::OR;
			if (words[1] == "XOR")
				op = Operation::XOR;
			if (words[1] == "AND")
				op = Operation::AND;
			gates[words[4]] = Gate{words[0], words[2], op, false, false};
		}

		// swapWires("qjj" , "gjc");
		// swapWires("qsb" , "z39");
		// swapWires("z17" , "wmp");
		// swapWires("gvm" , "z26");

		//gjc,gvm,qjj,qsb,wmp,z17,z26,z39
		auto sum = calculate(46);

		std::cout << sum << "\n";
		uint64_t x[10] {
			19570632878ull,
			1093652890446732ull,
			706869789648021ull,
			8681541956473ull,
			67947298627835467ull,
			736916797310742ull,
			284639567976256ull,
			7896532867513919ull,
			0b000000000000000000000000000000000000000000000,
			0b111111111111111111111111111111111111111111111
		};
		uint64_t y[10] {
			376292692938629ull,
			15698369ull,
			18935892688926ull,
			4576917310675ull,
			65739612860771ull,
			19865317687064ull,
			3965896165898ull,
			8635971991356ull,
			0b000000000000000000000000000000000000000000000,
			0b111111111111111111111111111111111111111111111
		};
		uint64_t realSum;
		uint64_t diff;
		uint64_t bitsum;

		//swapWires("z11", "z12");

		for (int i=0; i < 10; i++) {
			for (int j=0; j < 10; j++) {
				uint64_t x1 = x[i];
				uint64_t y1 = y[j];

				// uint64_t x1 = 1ull << i;
				// uint64_t y1 = 0b111111111111111111111111111111111111111111111ull;
				// if (j==0)
				// 	y1 = 0b000000000000000000000000000000000000000000000ull;

				sum = calculate(x1, y1, 45);
				realSum = x1 + y1;
				diff = sum ^ realSum;
				bitsum = x1 & y1;


				std::cout << i << "\n";
				std::cout << std::bitset<45>(x1) << "\n";
				std::cout << std::bitset<45>(y1) << "\n";
				std::cout << std::bitset<45>(bitsum) << "\n";
				std::cout << std::bitset<45>(diff) << "\n\n";
			}
		}


		//0 1 1 0
		//  +
		//1 0 1 1
		// z1 = x1 XOR x2

		//all zs are using correct input wtf

	}
};
