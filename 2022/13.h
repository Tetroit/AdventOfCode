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

//comparing and sorting packets

struct Task
{
	static inline std::vector<std::string> packets;

	enum struct CompRes {
		G,
		L,
		E
	};
	static CompRes compare (int a, int b) {
		if (b == a) return CompRes::E;
		if (b > a) return CompRes::G;
		return CompRes::L;
	}
	static int captureInt(const std::string& str, int& ptr) {
		int res = 0;
		while (true) {
			char ch = str[ptr];
			if (!std::isdigit(ch)) return res;
			ptr++;
			res = res * 10 + (ch - '0');
		}
	}
	static void closeList(const std::string& str, int& ptr) {
		int stack = 0;
		while (true) {
			char ch = str[ptr++];
			if (ch == '[') stack++;
			if (ch == ']') stack--;
			if (stack == -1) return;
		}
	}
	static CompRes compare(const std::string& a, int& aStart, const std::string& b, int& bStart) {
		bool aList = (a[aStart] == '[');
		bool bList = (b[bStart] == '[');
		bool aEmpty = aList ? (a[aStart+1] == ']') : false;
		bool bEmpty = bList ? (b[bStart+1] == ']') : false;
		if (aEmpty && bEmpty) {
			aStart+=2;
			bStart+=2;
			return CompRes::E;
		}
		if (aEmpty) {
			aStart+=2;
			if (bList) {
				bStart++;
				closeList(b, bStart);
			}
			else {
				captureInt(b, bStart);
			}
			return CompRes::G;
		}
		if (bEmpty) {

			if (aList) {
				aStart++;
				closeList(a, aStart);
			}
			else {
				captureInt(a, aStart);
			}
			return CompRes::L;
		}
		int aVal = 0;
		int bVal = 0;
		if (!aList && !bList) {
			int aVal = captureInt(a, aStart);
			int bVal = captureInt(b, bStart);
			return compare(aVal, bVal);
		}
		if (aList && !bList) {
			aStart++;
			auto res = compare(a, aStart, b, bStart);
			if (res == CompRes::E) {
				if (a[aStart] != ']') res = CompRes::L;
				else res = CompRes::E;
				closeList(a, aStart);
				return res;
			}

			closeList(a, aStart);
			return res;
		}
		if (!aList && bList) {
			bStart++;
			auto res = compare(a, aStart, b, bStart);
			if (res == CompRes::E) {
				if (b[bStart] != ']') res = CompRes::G;
				else res = CompRes::E;
				closeList(b, bStart);
				return res;
			}

			closeList(b, bStart);
			return res;
		}
		do {
			aStart++;
			bStart++;
			auto res = compare(a, aStart, b, bStart);
			if (res != CompRes::E) {
				closeList(a, aStart);
				closeList(b, bStart);
				return res;
			}
		} while (a[aStart] != ']' && b[bStart] != ']');
		if (a[aStart] == ']' && b[bStart] == ']') {
			closeList(a, aStart);
			closeList(b, bStart);
			return CompRes::E;
		}
		if (a[aStart] == ']') {
			closeList(a, aStart);
			closeList(b, bStart);
			return CompRes::G;
		}
		if (b[bStart] == ']') {
			closeList(a, aStart);
			closeList(b, bStart);
			return CompRes::L;
		}
	}
	static void run() {

		std::ifstream inputStream("2022/13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) continue;
			packets.emplace_back(line);
		}
		inputStream.close();

		int cnt = 0;
		for (int i=0; i<packets.size(); i+=2) {
			const auto& a = packets[i];
			const auto& b = packets[i+1];
			int ptr1 = 0, ptr2 = 0;
			if (compare(a, ptr1, b, ptr2) == CompRes::G) {
				cnt+=i/2+1;
			};
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		packets.emplace_back("[[2]]");
		packets.emplace_back("[[6]]");
		std::sort(packets.begin(), packets.end(), [](const std::string& a, const std::string& b) {
			int ptr1 = 0, ptr2 = 0;
			return compare(a, ptr1, b, ptr2) == CompRes::G;
		});
		// for (const auto& str : packets) {
		// 	std::cout << str << std::endl;
		// }
		int res = 1;
		for (int i=0; i<packets.size(); i++) {
			auto& str = packets[i];
			if (str == "[[2]]" || str == "[[6]]") res *= (i+1);
		}
		std::cout << res << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/