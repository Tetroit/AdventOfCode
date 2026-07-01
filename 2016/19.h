#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

struct Task
{
	static constexpr uint32_t size = 3012210;
	static void run() {
		//solved compile-time :fire:
		constexpr int res = ((size - std::bit_floor(size)) << 1) + 1;
		std::cout << res << std::endl;
	}
	static int floorb(int n, int base) {
		return n - n % base;
	}
	static void runPart2() {
		int remaining = size;
		int stride = 1;
		int phase = (size%2 == 0) ? 1 : 2;
		int firstSafe = remaining/2;
		while (remaining > 3) {
			//0 X X
			int startOffset = 3-phase;
			int rem3 = remaining%3;
			firstSafe = (firstSafe + startOffset*stride) % (int)size;
			phase = (phase + rem3)%3;
			remaining = (remaining-startOffset)/3 + 1;
			stride *= 3;
		}
		int res = firstSafe;
		if (remaining == 2 && phase != 0) {
			res += stride;
		}
		if (remaining == 3) {
			if (phase == 1) {
				res += stride;
			}
			if (phase == 2) {
				res += 2*stride;
			}
		}
		res%=size;
		std::cout << res + 1 << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

	lets start numbering from 0
	for each iteration we go around the circle
	gaps between elves rize as powers of 2 (neighbours have a gap of 1):
	XXXXXXXXXXXXXXXXXX 1
	X.X.X.X.X.X.X.X.X. 2
	X...X...X...X...X. 4
	X.......X.......X. 8
	X...............X. 16
	the only difference is when we reach the end
	if circle size is even
	then the last elf doesnt take a turn and we start a new cycle
	if it is odd
	then the first elf in the circle is excluded
	meaning the next cycle starts shifted by 2^n
	in the end the first elf in the cycle will get all the presents

	so the loop for each cycle can be written as
	while (remaining > 1) {
		stride<<=1;
		if (remaining & 1)
			res+=stride;
		remaining>>=1;
	}
	where stride is gap between next elves (excluding last <-> first)
	res is the first starting elf

	so effectively starting from low bits in input size
	and we copy this bit to a higher position into res like
	101110001 becomes 011100010
	because stride is 2^n, n is the cycle number
	except for the last bit, it is omitted
	because we only have 1 elf standing and we should not eliminate him

	this can be done by taking size, eliminating last bit and shifting everything by 1
	eliminating last bit can be done by taking floor in base2 and subtracting from size

	dont forget to add 1 since we start from 1 and not 0
*/