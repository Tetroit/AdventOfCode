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
#include "../utils.h"

struct Task
{

	struct program {

		long long A;
		long long B;
		long long C;
		std::vector<long long> input;
		std::vector<long long> output;
		long long GetCombo(long long code) const {
			if (0 <= code && code < 4)
				return code;
			if (code == 4)
				return A;
			if (code == 5)
				return B;
			if (code == 6)
				return C;
			std::cerr << "Invalid combo code" << '\n';
			return -1;
		}

		//2,4, - B = A mod 8
		//1,3, - B = B xor 3
		//7,5, - C = A / 2^B
		//0,3, - A = A >> 3
		//4,1, - B = B xor C
		//1,5, - B = B xor 5
		//5,5, - OUT <==== B mod 8
		//3,0, - return to start

		//A only shifts by 3 bits every loop, so we can backtrack it like 0bXXX -> 0bXXX|YYY -> 0bXXX|YYY|ZZZ comparing input to output
		//The number is between 2^input.size and 2^(input.size - 1)

		void ExecuteOperation(const std::vector<long long>& stream, int& id) {
			int code = stream[id];
			id++;
			long long val = stream[id];
			id++;
			switch (code) {
				case 0:
					val = GetCombo(val);
					A >>= val;
					return;
				case 1:
					B ^= val;
					return;
				case 2:
					B = GetCombo(val) & 0b111;
					return;
				case 3:
					if (A == 0)
						return;
					id = val;
				case 4:
					B ^= C;
					return;
				case 5:
					output.emplace_back(GetCombo(val) & 0b111);
					return;
				case 6:
					val = GetCombo(val);
					B = A >> val;
					return;
				case 7:
					val = GetCombo(val);
					C = A >> val;
					return;
				default: break;
			}
		}
		void Execute () {
			output.clear();
			int id = 0;
			while (id < input.size()) {
				ExecuteOperation(input, id);
			}
		}
		bool InputEqualOutput() {

			if (input.size() != output.size())
				return false;

			for (int i = 0; i < input.size() && i < output.size(); i++) {
				if (input[i] != output[i])
					return false;
			}
			return true;
		}

		long long SearchReplicate() {
			long long oldA = A;
			long long SampleA = 0;
			int digit = 0;
			while (SampleA < 1ll << (input.size() * 3)) {
				SampleA++;
				A = SampleA;
				Execute();
				if (output[0] == input[input.size() - digit - 1]) {
					if (InputEqualOutput())
						break;
					SampleA <<= 3ll;
					digit++;
					continue;
				}
				if ((SampleA & 7) == 7) {
					SampleA >>= 3;
					digit--;
				}
			}
			A = oldA;
			return SampleA;
		}

		std::string PrintOutput() {
			std::string res;
			for (long long& i : output) {
				res += std::to_string( i) + ',';
			}
			if (!res.empty())
				res.pop_back();
			return res;
		}

	};

	void run() {

		std::ifstream inputStream("input2024-17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::string line;
		std::vector<program> programs;
		while (std::getline(inputStream, line))
		{
			long long a; long long b; long long c;
			std::vector<long long> stream;
			std::stringstream(line.substr(line.find(':') + 1)) >> a;
			std::getline(inputStream, line);
			std::stringstream(line.substr(line.find(':') + 1)) >> b;
			std::getline(inputStream, line);
			std::stringstream(line.substr(line.find(':') + 1)) >> c;
			std::getline(inputStream, line);

			std::getline(inputStream, line);

			long long num;
			char comma;
			std::stringstream ss(line.substr(line.find(':') + 1));
			while (ss >> num) {
				stream.push_back(num);
				ss >> comma;
			}

			programs.emplace_back(program {a,b,c, stream,{}});
		}

		for (program& program : programs) {
			program.Execute();
			std::cout << program.PrintOutput() << '\n';
			long long replicate = program.SearchReplicate();
			std::cout << replicate << '\n';
		}

		//1,3,2,4,4,0,3,2,2

	}
};
