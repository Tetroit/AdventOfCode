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

    const std::vector<std::string> codes{
        "ab","cd","pq","xy"
    };

    const std::vector<char> allowedVowels{
        'a','e','i','o','u'
    };

    bool isNice1 (std::string line) {
        char prev;
        char curr;
        bool hasDouble = false;
        bool hasForbiddenSequence = false;
        int vowels = 0;
        for (const auto& code : codes) {
            if (line.find(code) != std::string::npos) {
                hasForbiddenSequence = true;
            }
        }
        if (hasForbiddenSequence) return false;

        for (int i=0; i<line.size(); i++) {
            curr = line[i];

            if (std::find(allowedVowels.begin(), allowedVowels.end(), curr) != allowedVowels.end()) {
                vowels++;
            }
            if (i != 0 && curr == prev) hasDouble = true;

            prev = curr;
        }

        if (hasDouble && vowels >= 3)
            return true;
        return false;
    }

    bool isNice2 (std::string line) {
        char prev;
        char curr;
        bool hasDoubleSeq = false;
        bool hasSandwich = false;
        for (int i=0; i<line.size(); i++) {
            curr = line[i];
            if (i > 0 && i < line.size() - 2) {

                if (line.find(line.substr(i-1,2), i+1) != std::string::npos) {
                    hasDoubleSeq = true;
                }
            }
            if (i > 0 && i < line.size() - 1) {
                if (line[i+1] == prev) {
                    hasSandwich = true;
                }
            }
            prev = curr;
        }

        if (hasDoubleSeq && hasSandwich)
            return true;
        return false;
    }
	void run() {


		std::ifstream inputStream("05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

	    int cnt1 = 0;
	    int cnt2 = 0;
		char dir;
	    std::string line;

		while (std::getline(inputStream, line))
		{
		    if (isNice1(line))
		        cnt1++;
		    if (isNice2(line))
		        cnt2++;
		}
	    std::cout << cnt1 << "\n";
	    //249
	    std::cout << cnt2 << "\n";
	}
};
