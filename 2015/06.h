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
#include <regex>
#include "../utils.h"

/*
 * The task where you need to light points according to instructions like
 * turn on rectangle (0,1),(23,19)
 */
struct Task
{
    enum Instruction {
        TURN_ON,
        TURN_OFF,
        TOGGLE
    };
    static int count (const std::vector<std::vector<int>> &arr) {
        int cnt = 0;
        for (auto& i : arr) {
            for (auto& j : i) {
                cnt+=j;
            }
        }
        return cnt;
    }
    static Instruction getInstruction(const std::string& code) {
        if (code == "turn on") {
            return TURN_ON;
        }
        if (code == "turn off") {
            return TURN_OFF;
        }
        if (code == "toggle") {
            return TOGGLE;
        }
        throw std::invalid_argument("Invalid Instruction");
    }
    static void run() {

        std::ifstream inputStream("06.txt");

        if (!inputStream.is_open()) {
            std::cerr << "Failed to open file!\n";
        }

        std::vector<std::vector<int>> lamps;
        std::string line;

        std::regex pattern(R"(([a-zA-Z ]+) (\d+),(\d+) ([a-zA-Z ]+) (\d+),(\d+))");
        std::smatch match;

        for (int i=0; i<1000; i++) {
            lamps.emplace_back();
            for (int j = 0; j < 1000; j++) {
                lamps[i].push_back(0);
            }
        }

        int x1, x2, y1, y2;
        Instruction instruction;


        while (std::getline(inputStream, line))
        {
            if (std::regex_match(line, match, pattern)) {
                instruction = getInstruction(match[1].str());
                x1 = std::stoi(match[2].str());
                y1 = std::stoi(match[3].str());
                x2 = std::stoi(match[5].str());
                y2 = std::stoi(match[6].str());

                for (int i = x1; i <= x2; i++) {
                    for (int j = y1; j <= y2; j++) {
                        switch (instruction) {
                            case TURN_ON:
                                lamps[i][j] = 1;
                                break;
                            case TURN_OFF:
                                lamps[i][j] = 0;
                                break;
                            case TOGGLE:
                                lamps[i][j] = (lamps[i][j] == 1) ? 0 : 1;
                                break;
                        }
                    }
                }
            }
        }
        std::cout << count(lamps) << '\n';
    }

    static void runPart2() {
        std::ifstream inputStream("06.txt");

        if (!inputStream.is_open()) {
            std::cerr << "Failed to open file!\n";
        }

        std::vector<std::vector<int>> lamps;
        std::string line;

        std::regex pattern(R"(([a-zA-Z ]+) (\d+),(\d+) ([a-zA-Z ]+) (\d+),(\d+))");
        std::smatch match;

        for (int i=0; i<1000; i++) {
            lamps.emplace_back();
            for (int j = 0; j < 1000; j++) {
                lamps[i].push_back(0);
            }
        }

        int x1, x2, y1, y2;
        Instruction instruction;


        while (std::getline(inputStream, line))
        {
            if (std::regex_match(line, match, pattern)) {
                instruction = getInstruction(match[1].str());
                x1 = std::stoi(match[2].str());
                y1 = std::stoi(match[3].str());
                x2 = std::stoi(match[5].str());
                y2 = std::stoi(match[6].str());

                for (int i = x1; i <= x2; i++) {
                    for (int j = y1; j <= y2; j++) {
                        switch (instruction) {
                            case TURN_ON:
                                lamps[i][j]++;
                                break;
                            case TURN_OFF:
                                lamps[i][j]--;
                                if (lamps[i][j] < 0)
                                    lamps[i][j] = 0;
                                break;
                            case TOGGLE:
                                lamps[i][j]+=2;
                                break;
                        }
                    }
                }
            }
        }
        std::cout << count(lamps) << '\n';
    }
};
