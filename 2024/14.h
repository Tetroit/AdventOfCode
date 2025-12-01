#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

//I used glm library form this task, 
#include "glm/glm.hpp"

struct Task
{
	char field[10506];
	void displayRobots(std::vector<glm::ivec2> pos) {

		for (int i = 0; i < 103; i++) {
			for (int j = 0; j < 101; j++)
			{
				field[j + 102 * i] = ' ';
			}
			field[101 + 102 * i] = '\n';
		}
		for (glm::ivec2 p : pos)
		{
			field[p.x + 102 * p.y] = '0';
		}
		std::cout << field;
	}

	void run() {

		glm::ivec2 room(101, 103);
		std::vector<glm::ivec2> pos;
		std::vector<glm::ivec2> vel;

		std::string line;
		std::ifstream inputStream("input2024-14.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		int p1 = 0, p2 = 0, v1 = 0, v2 = 0;

		int entries = 0;
		while (inputStream)
		{
			entries++;
			std::getline(inputStream, line);
			std::string snum = "";
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] == 'p')
				{
					i += 2;
					while (line[i] != ',')
						snum.append(&line[i++], 1);
					p1 = std::stoi(snum);
					snum = "";
					i++;
					while (line[i] != ' ')
						snum.append(&line[i++], 1);
					p2 = std::stoi(snum);
					snum = "";
					pos.push_back(glm::ivec2(p1, p2));
				}

				if (line[i] == 'v')
				{
					i += 2;
					while (line[i] != ',')
						snum.append(&line[i++], 1);
					v1 = std::stoi(snum);
					snum = "";
					i++;
					while (line[i] != '\0')
						snum.append(&line[i++], 1);
					v2 = std::stoi(snum);
					snum = "";
					vel.push_back(glm::ivec2(v1, v2));
				}
			}


		}

		int quads[4]{ 0,0,0,0 };
		for (int i = 0; i < pos.size(); i++)
		{
			glm::ivec2 endPos = pos[i] - vel[i] * glm::ivec2(1, 3);
			//glm::ivec2 endPos = pos[i] + vel[i] * 100;
			if (endPos.x < 0) {
				endPos.x -= (endPos.x / 101 - 1) * 101;
				if (endPos.x == 101)
					endPos.x = 0;
			}
			else if (endPos.x > 100) endPos.x = endPos.x % 101;

			if (endPos.y < 0) {
				endPos.y -= (endPos.y / 103 - 1) * 103;
				if (endPos.y == 103)
					endPos.y = 0;
			}

			else if (endPos.y > 102) endPos.y = endPos.y % 103;

			if (endPos.x < 50 && endPos.y < 51)
				quads[0]++;

			if (endPos.x < 50 && endPos.y > 51)
				quads[1]++;

			if (endPos.x > 50 && endPos.y < 51)
				quads[2]++;

			if (endPos.x > 50 && endPos.y > 51)
				quads[3]++;

			std::cout << endPos.x << " " << endPos.y << '\n';
		}

		std::cout << quads[0] * quads[1] * quads[2] * quads[3] << '\n';
		int stepsTaken = 0;
		int deltaTime = 0;
		int exDeltaTime = 0;
		bool scroll = false;
		while (true)
		{
			displayRobots(pos);
			std::cout << "\n\n\n" << stepsTaken << "\n\n\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

			if (!scroll)
			{
				exDeltaTime = deltaTime;
				std::cin >> deltaTime;
			}
			if (deltaTime == 0) {
				scroll = true;
				deltaTime = exDeltaTime;
			}

			stepsTaken += deltaTime;

			for (int i = 0; i < pos.size(); i++)
			{
				pos[i] += deltaTime * vel[i];
				if (pos[i].x < 0) {
					pos[i].x -= (pos[i].x / 101 - 1) * 101;
					if (pos[i].x == 101)
						pos[i].x = 0;
				}
				else if (pos[i].x > 100) pos[i].x = pos[i].x % 101;

				if (pos[i].y < 0) {
					pos[i].y -= (pos[i].y / 103 - 1) * 103;
					if (pos[i].y == 103)
						pos[i].y = 0;
				}
				else if (pos[i].y > 102) pos[i].y = pos[i].y % 103;
			}

		}
	}
	//236628054
};