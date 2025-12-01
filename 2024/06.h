#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "../utils.h"

struct Task
{
	enum class Cell : uint8_t {

		EMPTY = 0x0,
		WALKED = 0x1,
		OBSTACLE = 0x2
	};

	struct AdvancedCell {
		Cell cell : 3;
		bool traversedUp : 1;
		bool traversedDown : 1;
		bool traversedRight : 1;
		bool traversedLeft : 1;
		bool isPotentialObstacle : 1;

		Cell iCell : 3;
		bool iTraversedUp : 1;
		bool iTraversedDown : 1;
		bool iTraversedRight : 1;
		bool iTraversedLeft : 1;

		AdvancedCell() : AdvancedCell(Cell::EMPTY){}

		AdvancedCell(Cell base)
		{
			cell = base;
			traversedUp = false;
			traversedDown = false;
			traversedRight = false;
			traversedLeft = false;
			isPotentialObstacle = false;

			iCell = Cell::EMPTY;
			iTraversedUp = false;
			iTraversedDown = false;
			iTraversedRight = false;
			iTraversedLeft = false;
		}

		Cell GetCell(bool isImaginary = false) const
		{
			return isImaginary ? iCell : cell;
		}
		void RecordDir(int deltaX, int deltaY, bool isImaginary = false)
		{
			if (isImaginary && deltaX == 0 && deltaY == -1)
				iTraversedUp = true;
			if (isImaginary && deltaX == 0 && deltaY == 1)
				iTraversedDown = true;
			if (isImaginary && deltaX == 1 && deltaY == 0)
				iTraversedRight = true;
			if (isImaginary && deltaX == -1 && deltaY == 0)
				iTraversedLeft = true;

			if (!isImaginary && deltaX == 0 && deltaY == -1)
				traversedUp = true;
			if (!isImaginary && deltaX == 0 && deltaY == 1)
				traversedDown = true;
			if (!isImaginary && deltaX == 1 && deltaY == 0)
				traversedRight = true;
			if (!isImaginary && deltaX == -1 && deltaY == 0)
				traversedLeft = true;

		}
		bool AlreadyTraversed(int deltaX, int deltaY, bool isImaginary = false) const 
		{
			if ((cell != Cell::WALKED) && (iCell != Cell::WALKED))
				return false;

			if (deltaX == 0 && deltaY == 1 && (isImaginary ?
				(traversedDown || iTraversedDown) :
				traversedDown))
				return true;

			if (deltaX == 0 && deltaY == -1 && (isImaginary ?
				(traversedUp || iTraversedUp) :
				traversedUp))
				return true;

			if (deltaX == 1 && deltaY == 0 && (isImaginary ?
				(traversedRight || iTraversedRight) :
				traversedRight))
				return true;

			if (deltaX == -1 && deltaY == 0 && (isImaginary ?
				(traversedLeft || iTraversedLeft) :
				traversedLeft))
				return true;

			return false;
		}
		bool AlreadyStepped(bool isImaginary = false)
		{
			if (!isImaginary)
			{
				return (traversedDown || traversedUp || traversedRight || traversedLeft);
			}
			else
			{
				return (traversedDown || traversedUp || traversedRight || traversedLeft) ||
					(iTraversedDown || iTraversedUp || iTraversedRight || iTraversedLeft);
			}
		}
	};






	void turn(int& deltaX, int& deltaY)
	{
		int temp = deltaX;
		deltaX = -deltaY;
		deltaY = temp;
	}
	bool check_bounds(int posX, int posY, int width, int height) 
	{
		return (posX >= 0 && posY >= 0 && posX < width && posY < height);
	}
	int count_walked(Cell** map, int width, int height)
	{
		int cnt = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				if (map[i][j] == Cell::WALKED) {
					cnt++;
				}
			}
		}
		return cnt;
	}
	int count_walked(AdvancedCell** map, int width, int height)
	{
		int cnt = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				if (map[i][j].cell == Cell::WALKED) {
					cnt++;
				}
			}
		}
		return cnt;
	}
	int count_potential_obstacles(AdvancedCell** map, int width, int height)
	{
		int cnt = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				if (map[i][j].isPotentialObstacle) {
					cnt++;
				}
			}
		}
		return cnt;

	}

	bool will_loop(AdvancedCell** map, int width, int height, int deltaX, int deltaY, int posX, int posY) {


		int nextPosX = posX + deltaX;
		int nextPosY = posY + deltaY;
		if (!check_bounds(nextPosX, nextPosY, width, height) || map[nextPosX][nextPosY].cell == Cell::OBSTACLE)
		{
			return false;
		}

		//keep track of changes to revert it later
		std::vector<AdvancedCell*> imaginaryStack;
		imaginaryStack.push_back(&map[posX][posY]);

		//put an imaginary obstacle
		map[posX + deltaX][posY + deltaY].iCell = Cell::OBSTACLE;
		AdvancedCell* candidate = &map[posX + deltaX][posY + deltaY];
		turn(deltaX, deltaY);

		bool res = true;


		int overflowLock = width * height * 4;
		int steps = 0;

		while (true)
		{
			if (!check_bounds(posX, posY, width, height)) {
				res = false;
				break;
			}
			steps++;
			if (steps > overflowLock)
			{
				std::cout << "PROGRAM LOOPED";
				res = false;
				break;
			}

			// if we already went through that cell in that direction any time before, 
			// then we returned to a previous situation, 
			// which happens if and only if the path loops

			if (map[posX][posY].AlreadyTraversed(deltaX, deltaY, true))
			{
				break;
			}

			//record the current status
			map[posX][posY].RecordDir(deltaX, deltaY, true);

			if (map[posX][posY].iCell != Cell::WALKED)
			{
				map[posX][posY].iCell = Cell::WALKED;
			}


			int nextPosX = posX + deltaX;
			int nextPosY = posY + deltaY;
			if (!check_bounds(nextPosX, nextPosY, width, height))
			{
				res = false;
				break;
			}

			//case 1: turn
			if (map[nextPosX][nextPosY].cell == Cell::OBSTACLE || 
				map[nextPosX][nextPosY].iCell == Cell::OBSTACLE)
			{
				turn(deltaX, deltaY);
			}

			//case 2: move
			else
			{
				posX = nextPosX;
				posY = nextPosY;
				imaginaryStack.push_back(&map[posX][posY]);
			}
		}
		
		for (auto* cell : imaginaryStack)
		{
			cell->iCell = Cell::EMPTY;
			cell->iTraversedDown = false;
			cell->iTraversedUp = false;
			cell->iTraversedRight = false;
			cell->iTraversedLeft = false;
		}
		candidate->iCell = Cell::EMPTY;
		return res;
	}
	void simulate_with_obstacles(AdvancedCell** map, int deltaX, int deltaY, int posX, int posY, int width, int height)
	{

		int overflowLock = width * height * 4;
		int steps = 0;

		while (check_bounds(posX, posY, width, height))
		{

			steps++;
			if (steps > overflowLock)
			{
				std::cout << "PROGRAM LOOPED";
				return;
			}

			//record the current status
			map[posX][posY].RecordDir(deltaX, deltaY);

			if (map[posX][posY].cell != Cell::WALKED)
			{
				map[posX][posY].cell = Cell::WALKED;
			}

			int nextPosX = posX + deltaX;
			int nextPosY = posY + deltaY;

			//case 1: turn
			if (!check_bounds(nextPosX, nextPosY, width, height))
			{
				break;
			}
			if (map[nextPosX][nextPosY].cell == Cell::OBSTACLE)
			{
				turn(deltaX, deltaY);
			}

			//case 2: move
			else
			{
				//every time the guard is about to move forward we chech what if there was an obstacle
				//if guard already was there, then skip it, cuz otherwise she would've hit it in the past
				//from there, all changes made to the board are purely imaginary

				if (!map[nextPosX][nextPosY].AlreadyStepped())
				{
					if (will_loop(map, width, height, deltaX, deltaY, posX, posY))
					{
						map[posX + deltaX][posY + deltaY].isPotentialObstacle = true;
					}
				}
				posX = nextPosX;
				posY = nextPosY;
			}

		}
	}





	const std::string mapLegend = ".0#^";
	const int MAX_SIZE = 200;

	AdvancedCell** map;
	void run() {

		std::ifstream inputStream("input2024-06.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		map = new AdvancedCell*[MAX_SIZE];
		for (int i = 0; i < MAX_SIZE; i++)
		{
			map[i] = new AdvancedCell[MAX_SIZE];
		}

		char ch = 0;
		int row = 0;
		int col = 0;
		int width = 0;
		int height = 0;
		int guardX = 0;
		int guardY = 0;
		std::vector<AdvancedCell> imaginaryStack;

		while (inputStream >> std::noskipws >> ch)
		{
			switch (ch) {
			case '\n':
				col++;
				width = row;
				row = 0;
				continue;
			case '.':
				map[row][col].cell = Cell::EMPTY;
				break;
			case '#':
				map[row][col].cell = Cell::OBSTACLE;
				break;
			case '^':
				map[row][col].cell = Cell::EMPTY;
				guardX = row;
				guardY = col;
				break;
			}

			row++;
		}

		col++;

		height = col;

		simulate_with_obstacles(map, 0, -1, guardX, guardY, width, height);
		std::cout << count_walked(map, width, height) << "\n\n";
		std::cout << count_potential_obstacles(map, width, height) << "\n\n";

		//display field if you want
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (map[j][i].iCell != Cell::EMPTY)
					std::cout << '!';
				if (map[j][i].isPotentialObstacle)
					std::cout << 'X';
				else if ((map[j][i].traversedDown || map[j][i].traversedUp) && (map[j][i].traversedRight || map[j][i].traversedLeft))
					std::cout << '+';
				else if (!(map[j][i].traversedDown || map[j][i].traversedUp) && (map[j][i].traversedRight || map[j][i].traversedLeft))
					std::cout << '-';
				else if ((map[j][i].traversedDown || map[j][i].traversedUp) && !(map[j][i].traversedRight || map[j][i].traversedLeft))
					std::cout << '|';
				else
					std::cout << mapLegend[(int)(map[j][i].cell)];
			}
			std::cout << '\n';
		}

		for (int i = 0; i < MAX_SIZE; i++)
		{
			delete map[i];
		}
		delete map;
	}
};
