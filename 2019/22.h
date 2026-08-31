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

//shuffling card deck

enum struct Operation {
	FLIP,
	STRIDE,
	OFFSET,
};
struct GeneralOperation {
	long long stride = 1;
	long long offset = 0;
	long long size = 1;
	GeneralOperation(Operation op, long long arg, long long size) : size(size) {
		switch (op) {
			case Operation::FLIP:
				stride = -1;
				offset = -1;
				break;
			case Operation::STRIDE:
				stride = arg;
				break;
			case Operation::OFFSET:
				offset = -arg;
				break;
		}
	}
	GeneralOperation() = default;
	GeneralOperation (long long _offset, long long _stride, long long _size) : offset(_offset), stride(_stride), size(_size) {}
	explicit GeneralOperation (long long _size) : size(_size) {}

	void operator+=(const GeneralOperation& _other) {
		if (_other.size != size) {
			std::cerr << "size mismatch: left = " << size << "; right = " << _other.size << "\n";
		};
		offset = Utils::FloorMod(safeMultMod(offset,_other.stride,size) + _other.offset, size);
		stride = Utils::FloorMod(safeMultMod(stride,_other.stride,size), size);

	}
	GeneralOperation operator^(long long pow) const {
		GeneralOperation base = *this;
		GeneralOperation res(size);

		while (pow) {
			if (pow & 1)
				res += base;
			base += base;
			pow >>= 1;
		}

		return res;
	}

	long long operator()(const long long& n) const{
		return Utils::FloorMod(offset + safeMultMod(n, stride, size), size);
	}
	GeneralOperation reverse() const {
		GeneralOperation res(size);
		//s' = (m*size+1)/s
		long long mask = 0;
		if (stride == -1) res.stride = -1;
		else if (stride == 1) res.stride = 1;
		else {
			while (true) {
				if ((mask + 1) % stride == 0) {
					res.stride = (mask + 1)/stride;
					break;
				}
				mask += size;
			}
		}
		//o'=-o*s'%size
		res.offset = safeMultMod(-offset, res.stride, size);
		return res;
	}
private:
	static long long safeMultMod(long long a, long long b, long long mod) {
		bool negate = (a < 0) != (b < 0);
		a = abs(a) % mod;
		b = abs(b) % mod;
		long long res = 0;
		while (b > 0) {
			if (b & 1) {
				res = (res + a) % mod;
			}

			a = (a + a) % mod;
			b >>= 1;
		}
		return negate ? -res : res;
	}
};

struct Task
{
	static constexpr long long deck = 10007;
	static constexpr long long deck2 = 119315717514047;
	static constexpr long long target = 2019;
	static constexpr long long target2 = 2020;
	static constexpr long long repeats = 101741582076661;

	static inline std::vector<std::pair<Operation, long long>> instructions;
	static void run() {

		std::ifstream inputStream("2019/22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::istringstream ss(line);
			std::string token;
			int value = 0;
			ss>>token;
			if (token == "cut") {
				ss >> value;
				instructions.emplace_back(Operation::OFFSET, value);
				continue;
			}
			ss>>token;
			if (token == "with") {
				ss.ignore(sizeof("increment ")-1);
				ss >> value;
				instructions.emplace_back(Operation::STRIDE, value);
				continue;
			}
			instructions.emplace_back(Operation::FLIP, 0);
		}
		inputStream.close();

		GeneralOperation shuffle(deck);
		for (const auto& [op, val] : instructions) {
			shuffle+=GeneralOperation(op, val, deck);
		}
		std::cout << shuffle(target) << std::endl;
	}

	static void runPart2() {
		GeneralOperation shuffle(deck2);
		GeneralOperation antishuffle(deck2);
		for (int i=instructions.size()-1; i >= 0; i--) {
			const auto& [op, val] = instructions[i];
			antishuffle+=GeneralOperation(op, val, deck2).reverse();
		}
		auto ultimateShuffle = antishuffle^repeats;
		std::cout << ultimateShuffle(target2) << std::endl;
	}
	//68328631589228 high
	//100383992519458 high
};

//-------------- NOTES AREA ----------------
/*
	shuffle process is bijective, so there is only 1 state, shuffling which would give us initial state.
	best guess is the shuffle actually loops and we dont have to go through all the 1 morbillion iterations
	if thats the case there is going to be a state which gives the initial one.
	moreover we only care about Nth element. Other elements may be shuffled between iterations, but that will not change the result
	so we can define "same state" as any 2 states where Nth element positions are the same, this increases chances to get into the loop

	thats not the case ._.

	okay all sequences can be unified into a single operation
	n' = s * n + o % size
	(almost like 1D affine transform but modulo'd)
	have 2 params: s(stride), o(offset)
	and these operations (with fixed size) form a group
	meaning combination of any 2 ops of this format always give an op of this format
	so if
	OP12 = OP1 * OP2
	then parameters of the combined operation are
	s12=(s1*s2)%size
	o12=(o1*s2+o2)%size
	(size = const)

	example (mod excluded)

	s1=2 o1=3
	|||0|1|2|3|4|5|6|7
	s2=3 o2=4
	....|..|..|..0..|..1..|..2..|..3
	.............0.....1.....2.....3
	s12=6 o12=13

	identity operation
	s = 1
	o = 0

	reverse operation
	m*size+1 = s*s'
	s' = (m*size+1)/s
	m*size = o'+o*s'
	o' = m*size - o*s'

	for part 2, since all operations in input have low stride (up to 10)
	we can cut some slack and bruteforce s' by just incrementing m
*/