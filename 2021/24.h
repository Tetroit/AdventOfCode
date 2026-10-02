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

//Assembly simulator

struct Task
{
	static inline const std::vector<int> a1 {14,10,13,-8,11,11,14,-11,14,-1,-8,-5,-16,-6};
	static inline const std::vector<int> a2 {12,9,8,3,0,11,10,13,3,10,10,14,6,5};

	static inline std::unordered_map<char, int> reg;

	static int reduceTillFail(uint64_t num) {
		int res = -1;
		std::vector<int> digits(14);
		for (int i=0; i<14; i++) {
			if (num % 10 == 0) return 13-i;
			digits[13-i] = int(num%10);
			num/=10;
		}
		uint64_t z = 0;
		for (int i=0; i<14; i++) {
			int digit = digits[i];
			bool check = z % 26 + a1[i] != digit;
			if (a1[i] < 0) {
				z/=26;
				if (check) return i;
			}
			else
				z = z * 26 + a2[i] + digit;
		}
		return res;
	}

	[[noreturn]] static void run() {
		uint64_t code = 99999999999999ULL;
		std::vector<uint64_t> pow10(14);
		pow10[0] = 1;
		for (int i=1; i<14; i++) {
			pow10[i] = pow10[i-1] * 10;
		}
		do {
			int failId = reduceTillFail(code);
			if (failId == -1) {
				std::cout << code << std::endl;
				break;
			}
			uint64_t inc = pow10[13-failId];
			code-=inc;
		} while (true);
	}
	static void runPart2() {
		uint64_t code = 11111111111111ULL;

		std::vector<uint64_t> pow10(14);
		pow10[0] = 1;
		for (int i=1; i<14; i++) {
			pow10[i] = pow10[i-1] * 10;
		}
		do {
			int failId = reduceTillFail( code);
			if (failId == -1) {
				std::cout << code << std::endl;
				break;
			}
			uint64_t inc = pow10[13-failId];
			code+=inc;
		} while (true);
	}
};

//-------------- NOTES AREA ----------------
/*

inp w		w=1..9
mul x 0		x=0
add x z
mod x 26
div z 1
add x 14	x=14
eql x w		x=0
eql x 0		x=1
mul y 0
add y 25	y=25
mul y x		y=25
add y 1		y=26
mul z y		z=0
mul y 0		y=0
add y w		y=w
add y 12	y=w+12
mul y x		y=w+12
add z y		z=w+12

prev = z = w+12

inp w		w=1..9
mul x 0		x=0
add x z
mod x 26	x=prev%26
div z 1		z/=1
add x 10	x=prev%26+10
eql x w		x=0
eql x 0		x=1
mul y 0
add y 25	y=25
mul y x		y=25
add y 1		y=26
mul z y		z=prev * 26
mul y 0
add y w		y=w
add y 9		y=w+9
mul y x
add z y

if (z % 26 + 10 != in ) {
	z = z * 26 + 9 + in;

inp w
mul x 0
add x z
mod x 26
div z 26
add x -8
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 3
mul y x
add z y

//only if check is negative

z/=26

so every part is

bool check = z % 26 + a1 == in
if (a1 < 0) z/=26;
if (check) z = z * 26 + a2 + in;

since the only way to remove symbol is to get negative a1
and 7 times it is negative and 7 times it is negative
we need that the condition is true for every negative to compensate for expansion,
other numbers dont matter, leave them at 9 for the answer

*/