#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "utils.h"

struct Task
{
	int search_command(const std::string& str, const char* key, int till) {
		int latest_found_id = -1;
		int approved_id = -1;

		latest_found_id = str.find(key, 0);
		while (latest_found_id != std::string::npos && latest_found_id < till) {

			approved_id = latest_found_id;
			//std::cout << "Command " << key << " found at " << approved_id << '\n';
			latest_found_id = str.find(key, approved_id + std::strlen(key));

		}
		if (approved_id >= 0)
			std::cout << "Last command " << key << " found at " << approved_id << '\n';
		return approved_id;
	}
	int search_do_command(const std::string& str, int till) {
		return search_command(str, "do()", till);
	}
	int search_dont_command(const std::string& str, int till) {
		return search_command(str, "don't()", till);
	}
	void run() {

		std::string str;
		std::ifstream inputStream("input2024-03.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		int mult = 0;
		bool enabled = true;
		while (inputStream)
		{
			std::getline(inputStream, str);
			int id = str.find("mul(");

			while (id != std::string::npos)
			{
				//task 2
				int do_command = search_do_command(str, id);
				int dont_command = search_dont_command(str, id);

				if (do_command > -1 && dont_command > -1) {
					if (do_command > dont_command)
						enabled = true;
					else
						enabled = false;
				}
				else if (do_command > -1)
					enabled = true;
				else if (dont_command > -1)
					enabled = false;
				// task 2 end

				str = str.substr(id + 4, str.size() - id - 4);
				int comma = str.find(',');
				int bracket = str.find(')');
				if (comma != std::string::npos && bracket != std::string::npos && enabled)
				{
					std::string num1str = str.substr(0, comma);
					std::string num2str = str.substr(comma + 1, bracket - comma - 1);
					//std::string fullExpr = "mul(" + str.substr(0, bracket+1);
					//std::cout << fullExpr << "\n";

					try {
						size_t pos = 0;
						int num1 = std::stoi(num1str, &pos);
						if (pos != num1str.size())
							throw std::invalid_argument("excessive characters");

						int num2 = std::stoi(num2str, &pos);
						if (pos != num2str.size())
							throw std::invalid_argument("excessive characters");

						mult += num1 * num2;
						
					}
					catch (const std::invalid_argument&) {
						//std::cout << "NaN skip" << '\n';
					}
				}
				id = str.find("mul(");
			}
		}
		std::cout << mult << '\n';
	}
};
